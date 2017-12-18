
function code_init() {
	for (i = 1; (box = document.getElementById('code' + i)) != null; i++) {
		b = box.firstChild;
		c = box.lastChild;
		b.style.display = 'block';
		c.style.display = 'none';
		c.style.marginTop = '20px';
	}
}

function code_toggle(i) {
	box = document.getElementById('code' + i);
	b = box.firstChild;
	c = box.lastChild;
	if (c.style.display == 'block') {
		b.textContent = 'コードを表示する';
		c.style.display = 'none';
	} else {
		b.textContent = 'コードを非表示にする';
		c.style.display = 'block';
	}
}

code_init();