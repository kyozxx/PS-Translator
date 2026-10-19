(() => {
  const field = document.getElementById('game-search');
  const results = document.getElementById('game-results');
  const state = document.getElementById('game-state');
  const fold = text => text.normalize('NFD').replace(/\p{M}/gu, '').replace(/đ/gi, 'd').toLowerCase();
  let games = [], toastTimer;
  const reducedMotion = matchMedia('(prefers-reduced-motion: reduce)');
  const motionNodes = new Set();
  function reveal(element, delay = 0) {
    if (reducedMotion.matches || !element.animate) return;
    const animation = element.animate([
      { opacity: 0, transform: 'translateY(18px)' },
      { opacity: 1, transform: 'translateY(0)' }
    ], { duration: 560, delay, easing: 'cubic-bezier(.22,1,.36,1)', fill: 'backwards' });
    motionNodes.add(animation);
    animation.finished.catch(() => {}).finally(() => motionNodes.delete(animation));
  }
  reducedMotion.addEventListener('change', event => {
    if (event.matches) for (const animation of motionNodes) animation.cancel();
  });
  // The product appears first; the supporting text follows in reading order.
  document.querySelectorAll('.hero-copy > *').forEach((element, i) => reveal(element, 65 + i * 65));
  reveal(document.querySelector('.product-visual'), 150);
  if ('IntersectionObserver' in window) {
    const groups = document.querySelectorAll('.section-heading, .download, .tv-section, .steps > a, .search-row, .support-section > div');
    const observer = new IntersectionObserver(entries => {
      for (const entry of entries) if (entry.isIntersecting) {
        reveal(entry.target, Number(entry.target.dataset.motionDelay || 0));
        observer.unobserve(entry.target);
      }
    }, { threshold: 0.12 });
    document.querySelectorAll('.download, .steps > a').forEach((element, i) => { element.dataset.motionDelay = (i % 3) * 65; });
    groups.forEach(element => observer.observe(element));
  }
  document.querySelectorAll('.faq details').forEach(detail => {
    detail.addEventListener('toggle', () => { if (detail.open) reveal(detail.querySelector('p')); });
  });
  function notify(message) {
    const toast = document.getElementById('toast');
    toast.textContent = message; toast.classList.add('show');
    clearTimeout(toastTimer); toastTimer = setTimeout(() => toast.classList.remove('show'), 3000);
  }
  async function copy(name, button) {
    try { await navigator.clipboard.writeText(name); }
    catch {
      // Older browsers can still copy from a temporary selected field.
      const input = document.createElement('textarea'); input.value = name;
      input.setAttribute('readonly', ''); input.style.position = 'fixed'; input.style.left = '-9999px';
      document.body.append(input); input.select();
      const ok = document.execCommand('copy'); input.remove(); button.focus();
      if (!ok) { notify('Chưa sao chép được. Bạn có thể chọn và sao chép tên game trong danh sách.'); return; }
    }
    button.textContent = 'Đã sao chép';
    setTimeout(() => { button.textContent = 'Sao chép tên'; }, 1800);
    notify('Đã sao chép: ' + name);
  }
  function render() {
    const q = fold(field.value.trim());
    const found = games.filter(g => fold(g.name + ' ' + g.genre).includes(q));
    const shown = found.slice(0, q ? 40 : 4);
    state.textContent = !found.length ? 'Chưa có game này? Bấm Yêu cầu game để gửi tên cho quản trị viên.'
      : q ? `Tìm thấy ${found.length} game${found.length > shown.length ? '. Xem đầy đủ tại Danh sách game.' : ''}`
      : `${games.length} game có hướng dẫn dịch riêng · Đang hiển thị ${shown.length} game`;
    results.replaceChildren();
    for (const game of shown) {
      const row = document.createElement('article'); row.className = 'game-row';
      const detail = document.createElement('div');
      const name = document.createElement('strong'); name.textContent = game.name;
      const genre = document.createElement('small'); genre.textContent = game.genre;
      detail.append(name, genre);
      const button = document.createElement('button'); button.className = 'copy-button'; button.type = 'button';
      button.textContent = 'Sao chép tên'; button.setAttribute('aria-label', 'Sao chép tên ' + game.name);
      button.addEventListener('click', () => copy(game.name, button));
      row.append(detail, button); results.append(row);
    }
  }
  document.querySelectorAll('[data-demo-lang]').forEach(button => {
    button.addEventListener('click', () => {
      document.querySelector('.product-visual').dataset.language = button.dataset.demoLang;
      document.querySelectorAll('[data-demo-lang]').forEach(item => item.setAttribute('aria-pressed', String(item === button)));
    });
  });
  const phone = document.querySelector('.phone');
  const floatingPhone = phone.closest('.phone-float');
  let yaw = -10, pitch = 3, drag = null;
  const clamp = (value, limit) => Math.max(-limit, Math.min(limit, value));
  function rotatePhone() {
    yaw = clamp(yaw, 20); pitch = clamp(pitch, 12);
    phone.style.transform = `rotateX(${pitch}deg) rotateY(${yaw}deg) rotateZ(-2deg)`;
  }
  function resetPhone() { yaw = -10; pitch = 3; rotatePhone(); }
  phone.addEventListener('dragstart', event => event.preventDefault());
  phone.addEventListener('pointerdown', event => {
    if (event.button !== 0) return;
    phone.setPointerCapture(event.pointerId);
    drag = { id: event.pointerId, x: event.clientX, y: event.clientY, yaw, pitch };
    phone.classList.add('rotating');
    floatingPhone.classList.add('is-held');
  });
  phone.addEventListener('pointermove', event => {
    if (!drag || event.pointerId !== drag.id) return;
    yaw = drag.yaw + (event.clientX - drag.x) * 0.14;
    pitch = drag.pitch - (event.clientY - drag.y) * 0.1;
    rotatePhone();
  });
  function endRotation(event) {
    if (!drag || event.pointerId !== drag.id) return;
    drag = null; phone.classList.remove('rotating');
    floatingPhone.classList.remove('is-held');
    resetPhone();
    if (phone.hasPointerCapture(event.pointerId)) phone.releasePointerCapture(event.pointerId);
  }
  phone.addEventListener('pointerup', endRotation);
  phone.addEventListener('pointercancel', endRotation);
  phone.addEventListener('lostpointercapture', () => {
    drag = null; phone.classList.remove('rotating'); floatingPhone.classList.remove('is-held'); resetPhone();
  });
  phone.addEventListener('dblclick', resetPhone);
  phone.addEventListener('keydown', event => {
    if (!['ArrowLeft','ArrowRight','ArrowUp','ArrowDown','Home','Escape'].includes(event.key)) return;
    event.preventDefault();
    if (event.key === 'Home' || event.key === 'Escape') { resetPhone(); return; }
    if (event.key === 'ArrowLeft') yaw -= 4;
    if (event.key === 'ArrowRight') yaw += 4;
    if (event.key === 'ArrowUp') pitch += 3;
    if (event.key === 'ArrowDown') pitch -= 3;
    rotatePhone();
  });
  rotatePhone();
  field.addEventListener('input', render);
  fetch('games.json').then(r => { if (!r.ok) throw new Error('catalog'); return r.json(); })
    .then(data => { games = data; render(); })
    .catch(() => { state.textContent = 'Chưa tải được danh sách. Bấm “Xem toàn bộ danh sách” để mở trang game.'; });
})();
