/* ══════════════════════════════════════
   WMS — Auth Shared JavaScript
   Used by: login.html, signup.html
══════════════════════════════════════ */

const BASE = 'http://localhost/wms/';

/* ── PASSWORD EYE TOGGLE ── */
function toggleEye(inputId, iconId) {
  const inp    = document.getElementById(inputId);
  const icon   = document.getElementById(iconId);
  const isText = inp.type === 'text';
  inp.type = isText ? 'password' : 'text';
  icon.classList.toggle('fa-eye',       isText);
  icon.classList.toggle('fa-eye-slash', !isText);
}

/* ── ERROR HELPERS ── */
function setErr(errId, inputId, msg) {
  const e = document.getElementById(errId);
  e.textContent = msg;
  e.classList.add('show');
  document.getElementById(inputId).classList.add('err');
}

function clrErr(errId, inputId) {
  document.getElementById(errId).classList.remove('show');
  document.getElementById(inputId).classList.remove('err');
}

/* ── TOAST ── */
let _toastTimer;
function toast(msg, type) {
  const t = document.getElementById('toast');
  t.textContent = msg;
  t.className   = 'toast ' + type;
  clearTimeout(_toastTimer);
  _toastTimer = setTimeout(() => { t.className = 'toast'; }, 3200);
}
