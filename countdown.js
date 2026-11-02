// GTA VI launch countdown: fills [data-countdown] boxes every second; after the date it says the game is out.
(() => {
  document.querySelectorAll('[data-countdown]').forEach(box => {
    const end = Date.parse(box.dataset.countdown);
    const part = k => box.querySelector(`[data-${k}]`);
    const tick = () => {
      const left = Math.max(0, end - Date.now());
      if (!left) {
        box.classList.add('is-out');
        box.querySelector('.gc-title').textContent = box.dataset.outTitle;
        return clearInterval(timer);
      }
      const s = Math.floor(left / 1000);
      part('d').textContent = Math.floor(s / 86400);
      part('h').textContent = String(Math.floor(s / 3600) % 24).padStart(2, '0');
      part('m').textContent = String(Math.floor(s / 60) % 60).padStart(2, '0');
      part('s').textContent = String(s % 60).padStart(2, '0');
    };
    const timer = setInterval(tick, 1000);
    tick();
  });
})();
