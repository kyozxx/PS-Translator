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
})();

// Hero screenshots: slide by themselves (unless reduced motion), arrows and dots move them, tap opens the viewer.
(() => {
  const viewer = document.getElementById('shot-viewer');
  // Each group (app screenshots, member photos) pages through its own pictures.
  const groups = {};
  document.querySelectorAll('[data-shot]').forEach(a => (groups[a.dataset.group || 'app'] ||= new Map()).set(a.dataset.shot, a));
  let links = [];
  if (!viewer || !Object.keys(groups).length || !viewer.showModal) return;
  const img = viewer.querySelector('img'), count = viewer.querySelector('.sv-count');
  let at = 0;
  const show = i => {
    at = (i + links.length) % links.length;
    img.src = links[at].href;
    img.alt = links[at].querySelector('img').alt;
    count.textContent = `${at + 1} / ${links.length}`;
  };
  document.addEventListener('click', e => {
    const a = e.target.closest('[data-shot]');
    if (!a) return;
    e.preventDefault(); links = [...groups[a.dataset.group || 'app'].values()]; show(+a.dataset.shot); viewer.showModal();
  });
  viewer.querySelector('.sv-prev').onclick = () => show(at - 1);
  viewer.querySelector('.sv-next').onclick = () => show(at + 1);
  viewer.querySelector('.sv-close').onclick = () => viewer.close();
  viewer.addEventListener('click', e => { if (e.target === viewer) viewer.close(); });
  viewer.addEventListener('keydown', e => { if (e.key === 'ArrowLeft') show(at - 1); if (e.key === 'ArrowRight') show(at + 1); });
  let x0 = null;
  viewer.addEventListener('touchstart', e => { x0 = e.touches[0].clientX; }, { passive: true });
  viewer.addEventListener('touchend', e => {
    if (x0 === null) return;
    const dx = e.changedTouches[0].clientX - x0; x0 = null;
    if (Math.abs(dx) > 40) show(at + (dx < 0 ? 1 : -1));
  });
})();

(() => {
  const box = document.querySelector('.hero-shots');
  if (!box) return;
  const slides = [...box.querySelectorAll('.hs-track a')], dots = [...box.querySelectorAll('.hs-dots button')];
  const n = slides.length;
  let at = 0, timer = null;
  // Stacked cards: the current one on top, the previous and next tucked behind it to each side.
  const go = i => {
    at = (i + n) % n;
    slides.forEach((s, k) => {
      let d = (k - at + n) % n;
      if (d > n / 2) d -= n;
      const side = Math.abs(d) === 1;
      s.classList.toggle('on', d === 0);
      s.classList.toggle('side', side);
      s.style.transform = d === 0 ? 'none' : `translateX(${Math.sign(d) * (side ? 42 : 60)}%) scale(${side ? .84 : .7}) rotate(${Math.sign(d) * (side ? 4 : 6)}deg)`;
      s.style.zIndex = 3 - Math.min(Math.abs(d), 3);
      s.tabIndex = d === 0 ? 0 : -1;
    });
    dots.forEach((d, k) => d.setAttribute('aria-current', String(k === at)));
  };
  const stop = () => { clearInterval(timer); timer = null; };
  const play = () => { if (!timer && !matchMedia('(prefers-reduced-motion: reduce)').matches) timer = setInterval(() => go(at + 1), 3500); };
  box.querySelector('.hs-prev').onclick = () => { go(at - 1); stop(); };
  box.querySelector('.hs-next').onclick = () => { go(at + 1); stop(); };
  dots.forEach((d, k) => d.onclick = () => { go(k); stop(); });
  // A side card is a shortcut to it; only the top card opens the viewer.
  box.addEventListener('click', e => {
    const a = e.target.closest('.hs-track a');
    if (a && a.classList.contains('side')) { e.preventDefault(); e.stopPropagation(); go(slides.indexOf(a)); stop(); }
  }, true);
  box.addEventListener('mouseenter', stop);
  box.addEventListener('mouseleave', play);
  let x0 = null;
  box.addEventListener('touchstart', e => { x0 = e.touches[0].clientX; stop(); }, { passive: true });
  box.addEventListener('touchend', e => {
    if (x0 === null) return;
    const dx = e.changedTouches[0].clientX - x0; x0 = null;
    if (Math.abs(dx) > 40) go(at + (dx < 0 ? 1 : -1));
  });
  go(0); play();
})();

// Member feedback: on phones the cards sit in a swipe row; move to the next one every 4 s,
// back to the first after the last. A touch pauses it for a while.
(() => {
  const row = document.querySelector('.review-grid');
  if (!row || matchMedia('(prefers-reduced-motion: reduce)').matches) return;
  let pausedUntil = 0;
  row.addEventListener('touchstart', () => { pausedUntil = Date.now() + 8000; }, { passive: true });
  setInterval(() => {
    if (Date.now() < pausedUntil || row.scrollWidth <= row.clientWidth + 4) return;
    const card = row.firstElementChild, step = card.offsetWidth + (parseFloat(getComputedStyle(row).columnGap) || 0);
    const atEnd = row.scrollLeft + row.clientWidth >= row.scrollWidth - 4;
    // scrollBy, not scrollTo: smooth scrollTo to an offset is dropped inside a scroll-snap row.
    if (atEnd) row.scrollBy({ left: -row.scrollWidth, behavior: 'smooth' });
    else row.scrollBy({ left: step, behavior: 'smooth' });
  }, 4000);
})();
