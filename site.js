(() => {
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
})();
