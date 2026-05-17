// Auto-dismiss toast after 3 seconds
document.addEventListener('DOMContentLoaded', () => {
    const toast = document.querySelector('.toast');
    if (toast) {
        setTimeout(() => {
            toast.style.transition = 'opacity .4s';
            toast.style.opacity = '0';
            setTimeout(() => toast.remove(), 400);
        }, 3000);
    }
});
