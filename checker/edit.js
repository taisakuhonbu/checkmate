// チェックのサイズ
var sz = 64;
// 糸の配列
var warp;
var weft;
// チェックの描画エリア
const checkerAreaX = 16;
const checkerAreaY = 16;
const checkerAreaW = 256;
const checkerAreaH = 256;
// 糸玉を描画するエリア
const warpAreaX = 16;
const warpAreaY = checkerAreaY + checkerAreaH + 8;
const weftAreaX = checkerAreaX + checkerAreaW + 8;
const weftAreaY = 16;
const ballSize = 16;
const ballSlots = 16;
// 糸玉とチェックを結ぶ線を描画する部分の幅
const leading = 32;
// 織り方
var twillX = 1;
var twillY = 1;
// チェックの表示倍率
var scale = 4;
// 編集領域拡大用
var viewOffsetX = 0;
var viewOffsetY = 0;
var editOffsetX = 0;
var editOffsetY = 0;
// 色
var r = 192;
var g = 128;
var b = 64;
// canvas コンテキストチェック用と色指定用
var c;
var color;

// デフォルトのギンガムチェックを作る
function initChecker(prev)
{
	var i;
	for(i = prev; i < sz; )
	{
		warp[i] = [0,0,0,255];
		weft[i] = [0,0,0,255];
		i++;
		warp[i] = [0,0,0,255];
		weft[i] = [0,0,0,255];
		i++;
		warp[i] = [0,0,0,255];
		weft[i] = [0,0,0,255];
		i++;
		warp[i] = [0,0,0,255];
		weft[i] = [0,0,0,255];
		i++;
		warp[i] = [0,0,0,255];
		weft[i] = [0,0,0,255];
		i++;
		warp[i] = [0,0,0,255];
		weft[i] = [0,0,0,255];
		i++;
		warp[i] = [0,0,0,255];
		weft[i] = [0,0,0,255];
		i++;
		warp[i] = [0,0,0,255];
		weft[i] = [0,0,0,255];
		i++;
		warp[i] = [255,255,255,255];
		weft[i] = [255,255,255,255];
		i++;
		warp[i] = [255,255,255,255];
		weft[i] = [255,255,255,255];
		i++;
		warp[i] = [255,255,255,255];
		weft[i] = [255,255,255,255];
		i++;
		warp[i] = [255,255,255,255];
		weft[i] = [255,255,255,255];
		i++;
		warp[i] = [255,255,255,255];
		weft[i] = [255,255,255,255];
		i++;
		warp[i] = [255,255,255,255];
		weft[i] = [255,255,255,255];
		i++;
		warp[i] = [255,255,255,255];
		weft[i] = [255,255,255,255];
		i++;
		warp[i] = [255,255,255,255];
		weft[i] = [255,255,255,255];
		i++;
	}
}

// 初期化
function setup()
{
	// 糸の配列
	warp = new Array(sz);
	weft = new Array(sz);
	// チェックのデータが保存されていたら、それをロードする
	encoded = localStorage["source"];
	if(encoded)
	{
		// ロード実行
		var i;
		for(i = 0; i < sz; i++)
		{
			warp[i] = [0,0,0,255];
			weft[i] = [0,0,0,255];
		}
		try
		{
			decode();
			onEncode(null);
		}
		catch(e)
		{
			initChecker(0);
		}
	}
	else
	{
		initChecker(0);
	}

	var canvas;

	// 編集用キャンバスの準備
	canvas = document.getElementById("edit");
	c = canvas.getContext("2d");
	draw();
	canvas.addEventListener("mousedown", mouseDown);

	// 色指定用のキャンバスの準備
	canvas = document.getElementById("color");
	color = canvas.getContext("2d");
	drawColor();
	canvas.addEventListener("mousedown", changeColor0);
	canvas.addEventListener("mousemove", changeColor);
	canvas.addEventListener("mouseup", colorFixed);
	canvas.addEventListener("mouseout", colorFixed);
	
	var t;
	// 色データ表示
	t = document.getElementById("r");
	t.value = r;
	t.addEventListener("input", setR);
	t.addEventListener("change", setR);
	t = document.getElementById("g");
	t.value = g;
	t.addEventListener("input", setG);
	t.addEventListener("change", setG);
	t = document.getElementById("b");
	t.value = b;
	t.addEventListener("input", setB);
	t.addEventListener("change", setB);

	var select;
	// 織り方の select
	select = document.getElementById("twill");
	select.addEventListener("change", twillChange);
	
	var button;
	// エンコードとデコードのボタン
	button = document.getElementById("encode");
	button.addEventListener("click", onEncode);
	button = document.getElementById("decode");
	button.addEventListener("click", onDecode);
	// チェック画像を取得するボタン
	button = document.getElementById("check");
	button.addEventListener("click", onCheck);
	
	var image;
	// 経糸緯糸のユーティリティ
	image = document.getElementById("copydown");
	image.addEventListener("click", copydown);
	image = document.getElementById("swap");
	image.addEventListener("click", swap);
	image = document.getElementById("copyup");
	image.addEventListener("click", copyup);
	
	var sample;
	// サンプル
	sample = document.getElementById("sampleあ");
	sample.addEventListener("click", sampleあ);
	sample = document.getElementById("sample１");
	sample.addEventListener("click", sample１);
	sample = document.getElementById("sampleＡ");
	sample.addEventListener("click", sampleＡ);
}

// チェックの描画
function drawChecker()
{
	var bitmap = c.createImageData(checkerAreaW, checkerAreaH);
	var dx;
	var dy;
	var sx;
	var sy;
	var rx = 0;
	var ry = 0;
	var thread;
	sy = viewOffsetY;
	for(dy = 0; dy < checkerAreaH; dy++)
	{
		sx = viewOffsetX;
		thread = sy % (twillX + twillY) - twillY;
		for(dx = 0; dx < checkerAreaW; dx++)
		{
			if(thread < 0)
			{
				bitmap.data[(checkerAreaW * dy + dx) * 4 + 0] = warp[sx][0];
				bitmap.data[(checkerAreaW * dy + dx) * 4 + 1] = warp[sx][1];
				bitmap.data[(checkerAreaW * dy + dx) * 4 + 2] = warp[sx][2];
				bitmap.data[(checkerAreaW * dy + dx) * 4 + 3] = warp[sx][3];
			}
			else
			{
				bitmap.data[(checkerAreaW * dy + dx) * 4 + 0] = weft[sy][0];
				bitmap.data[(checkerAreaW * dy + dx) * 4 + 1] = weft[sy][1];
				bitmap.data[(checkerAreaW * dy + dx) * 4 + 2] = weft[sy][2];
				bitmap.data[(checkerAreaW * dy + dx) * 4 + 3] = weft[sy][3];
			}
			rx++;
			if(rx == scale)
			{
				sx++;
				rx = 0;
				thread++;
				if(thread == twillX)
				{
					thread = -twillY;
				}
			}
		}
		ry++;
		if(ry == scale)
		{
			sy++;
			ry = 0;
		}
	}
	c.putImageData(bitmap, checkerAreaX, checkerAreaY);
}

// 糸玉を描画
function drawYarnBall()
{
	var i;
	var p;
	var yarn;
	var ax;
	var ay;
	var sx;
	var sy;
	var bx;
	var by;
	var lx;
	var ly;

	c.fillStyle = "rgb(255,255,255)";
	c.fillRect(warpAreaX, warpAreaY, ballSize * ballSlots, leading);
	c.fillRect(weftAreaX, weftAreaY, leading, ballSize * ballSlots);
	
	yarn = warp;
	ax = warpAreaX;
	ay = warpAreaY;
	sx = scale;
	sy = 0;
	bx = ballSize;
	by = 0;
	lx = 0;
	ly = leading;
	for(p = 0; p < ballSlots; p++)
	{
		i = editOffsetX - viewOffsetX + p;
		c.fillStyle = "rgb("
			+ yarn[editOffsetX + p][0] + ","
			+ yarn[editOffsetX + p][1] + ","
			+ yarn[editOffsetX + p][2] + ")";
		c.beginPath();
		c.moveTo(
			ax + i * sx,
			ay + i * sy);
		c.lineTo(
			ax + p * bx + lx,
			ay + p * by + ly);
		c.lineTo(
			ax + p * bx + bx + lx,
			ay + p * by + by + ly);
		c.lineTo(
			ax + i * sx + sx,
			ay + i * sy + sy);
		c.closePath();
		c.fill();
		c.fillStyle = "rgb("
			+ (255 - yarn[editOffsetX + p][0]) + ","
			+ (255 - yarn[editOffsetX + p][1]) + ","
			+ (255 - yarn[editOffsetX + p][2]) + ")";
		c.fillRect(
			ax + p * bx + lx,
			ay + p * by + ly,
			ballSize, ballSize);
		c.fillStyle = "rgb("
			+ yarn[editOffsetX + p][0] + ","
			+ yarn[editOffsetX + p][1] + ","
			+ yarn[editOffsetX + p][2] + ")";
		c.fillRect(
			ax + p * bx + lx + 1,
			ay + p * by + ly + 1,
			ballSize - 2, ballSize -2);
	}
	yarn = weft;
	ax = weftAreaX;
	ay = weftAreaY;
	sx = 0;
	sy = scale;
	bx = 0;
	by = ballSize;
	lx = leading;
	ly = 0;
	for(p = 0; p < ballSlots; p++)
	{
		i = editOffsetY - viewOffsetY + p;
		c.fillStyle = "rgb("
			+ yarn[editOffsetY + p][0] + ","
			+ yarn[editOffsetY + p][1] + ","
			+ yarn[editOffsetY + p][2] + ")";
		c.beginPath();
		c.moveTo(
			ax + i * sx,
			ay + i * sy);
		c.lineTo(
			ax + p * bx + lx,
			ay + p * by + ly);
		c.lineTo(
			ax + p * bx + bx + lx,
			ay + p * by + by + ly);
		c.lineTo(
			ax + i * sx + sx,
			ay + i * sy + sy);
		c.closePath();
		c.fill();
		c.fillStyle = "rgb("
			+ (255 - yarn[editOffsetY + p][0]) + ","
			+ (255 - yarn[editOffsetY + p][1]) + ","
			+ (255 - yarn[editOffsetY + p][2]) + ")";
		c.fillRect(
			ax + p * bx + lx,
			ay + p * by + ly,
			ballSize, ballSize);
		c.fillStyle = "rgb("
			+ yarn[editOffsetY + p][0] + ","
			+ yarn[editOffsetY + p][1] + ","
			+ yarn[editOffsetY + p][2] + ")";
		c.fillRect(
			ax + p * bx + lx + 1,
			ay + p * by + ly + 1,
			ballSize - 2, ballSize -2);
	}
}

function drawArrow()
{
	c.fillStyle = "rgb(242,35,45)";
	c.beginPath();
	c.moveTo(checkerAreaX / 2, checkerAreaY);
	c.lineTo(0, checkerAreaY + checkerAreaY);
	c.lineTo(checkerAreaX, checkerAreaY + checkerAreaY);
	c.closePath();
	c.fill();
	c.beginPath();
	c.moveTo(checkerAreaX / 2, checkerAreaY + checkerAreaH);
	c.lineTo(checkerAreaX, checkerAreaH);
	c.lineTo(0, checkerAreaH);
	c.closePath();
	c.fill();
	c.beginPath();
	c.moveTo(checkerAreaX, checkerAreaY / 2);
	c.lineTo(checkerAreaX + checkerAreaX, checkerAreaY);
	c.lineTo(checkerAreaX + checkerAreaX, 0);
	c.closePath();
	c.fill();
	c.beginPath();
	c.moveTo(checkerAreaX + checkerAreaW, checkerAreaY / 2);
	c.lineTo(checkerAreaW, 0);
	c.lineTo(checkerAreaW, checkerAreaY);
	c.closePath();
	c.fill();
	
	c.fillStyle = "rgb(35,153,66)";
	c.beginPath();
	c.moveTo(0, warpAreaY + leading + ballSize / 2);
	c.lineTo(checkerAreaX, warpAreaY + leading + ballSize);
	c.lineTo(checkerAreaX, warpAreaY + leading);
	c.closePath();
	c.fill();
	c.beginPath();
	c.moveTo(checkerAreaX + ballSize * ballSlots + checkerAreaX, warpAreaY + leading + ballSize / 2);
	c.lineTo(checkerAreaX + ballSize * ballSlots, warpAreaY + leading);
	c.lineTo(checkerAreaX + ballSize * ballSlots, warpAreaY + leading + ballSize);
	c.closePath();
	c.fill();
	c.beginPath();
	c.moveTo(weftAreaX + leading + ballSize / 2, 0);
	c.lineTo(weftAreaX + leading, checkerAreaY);
	c.lineTo(weftAreaX + leading + ballSize, checkerAreaY);
	c.closePath();
	c.fill();
	c.beginPath();
	c.moveTo(weftAreaX + leading + ballSize / 2, checkerAreaY + ballSize * ballSlots + checkerAreaX);
	c.lineTo(weftAreaX + leading, checkerAreaY + ballSize * ballSlots);
	c.lineTo(weftAreaX + leading + ballSize, checkerAreaY + ballSize * ballSlots);
	c.closePath();
	c.fill();
}

// チェック編集部描画
function draw()
{
	drawChecker();
	drawYarnBall();
//	drawArrow();
}

// チェック編集部のマウスダウンイベントハンドラー
function mouseDown(e)
{
	var rect = e.target.getBoundingClientRect();
	var x =  Math.round(e.clientX - rect.left);
	var y =  Math.round(e.clientY - rect.top);
	
	if(x < checkerAreaX + checkerAreaW && y < checkerAreaY + checkerAreaH)
	{
		if(x < checkerAreaX)
		{
			if(y > checkerAreaY && y < checkerAreaY + checkerAreaY)
			{
				// scroll Y
			}
			if(y > checkerAreaH && y < checkerAreaY + checkerAreaH)
			{
				//scroll y
			}
			draw();
			return;
		}
		if(y < checkerAreaY)
		{
			if(x > checkerAreaX && x < checkerAreaX + chekcerAreaX)
			{
				//scroll x;
			}
			if(x > checkerAreaW && x < checkerAreaX + checkerAreaW)
			{
				//scroll y;
			}
			draw();
			return;
		}
		x -= checkerAreaX;
		y -= checkerAreaY;
		editOffsetX = Math.floor(x / scale / ballSlots) * ballSlots;
		editOffsetY = Math.floor(y / scale / ballSlots) * ballSlots;
		draw();
		return;
	}

	// 色指定部の色を使うのか、編集部から色を拾ってくるのか
	var isSet;
	var checkbox = document.getElementById("get");
	var t;
	if(checkbox.checked)
	{
		isSet = false;
	}
	else
	{
		isSet = true;
	}
	
	if(y > warpAreaY + leading)
	{
		x -= checkerAreaX;
		if(x < 0) return;
		x = Math.floor(x / ballSize);
		if(x > ballSlots) return;
		x += editOffsetX;
		if(isSet)
		{
			warp[x] = [r, g, b, 255];
			draw();
		}
		else
		{
			r = warp[x][0];
			g = warp[x][1];
			b = warp[x][2];
			t = document.getElementById("r");
			t.value = r;
			t = document.getElementById("g");
			t.value = g;
			t = document.getElementById("b");
			t.value = b;
			drawColor();
			checkbox = document.getElementById("set");
			checkbox.checked = true;
		}
		return;
	}
	if(x > weftAreaX + leading)
	{
		y -= checkerAreaY;
		if(y < 0) return;
		y = Math.floor(y / ballSize);
		if(y > ballSlots) return;
		y += editOffsetY;
		if(isSet)
		{
			weft[y] = [r, g, b, 255];
			draw();
		}
		else
		{
			r = weft[y][0];
			g = weft[y][1];
			b = weft[y][2];
			t = document.getElementById("r");
			t.value = r;
			t = document.getElementById("g");
			t.value = g;
			t = document.getElementById("b");
			t.value = b;
			drawColor();
			checkbox = document.getElementById("set");
			checkbox.checked = true;
		}
		return;
	}
}

// 色編集部を捜査した時のハンドラーの中身
function setR(e)
{
	var r0 = e.target.value;
	r = parseInt(r0);
	if(r0 == "" || r < 0)
	{
		r = 0;
		e.target.value = "0";
	}
	if(r > 255)
	{
		r = 255;
		e.target.value = "255";
	}
	drawColor();
}
function setG(e)
{
	var g0 = e.target.value;
	g = parseInt(g0);
	if(g0 == "" || g < 0)
	{
		g = 0;
		e.target.value = "0";
	}
	if(g > 255)
	{
		g = 255;
		e.target.value = "255";
	}
	drawColor();
}
function setB(e)
{
	var b0 = e.target.value;
	b = parseInt(b0);
	if(b0 == "" || b < 0)
	{
		b = 0;
		e.target.value = "0";
	}
	if(b > 255)
	{
		b = 255;
		e.target.value = "255";
	}
	drawColor();
}

// 色編集部を描画
function drawColor()
{
	var gr;
	gr = color.createLinearGradient(0, 0, 256, 0);
	gr.addColorStop(0, "#000000");
	gr.addColorStop(1, "#FF0000");
	color.fillStyle = gr;
	color.fillRect(0, 0, 256, 32);
	gr = color.createLinearGradient(0, 0, 256, 0);
	gr.addColorStop(0, "#000000");
	gr.addColorStop(1, "#00FF00");
	color.fillStyle = gr;
	color.fillRect(0, 32, 256, 32);
	gr = color.createLinearGradient(0, 0, 256, 0);
	gr.addColorStop(0, "#000000");
	gr.addColorStop(1, "#0000FF");
	color.fillStyle = gr;
	color.fillRect(0, 64, 256, 32);
	
	gr = color.createLinearGradient(0, 0, 256, 0);
	gr.addColorStop(0, "rgb(0," + g + "," + b + ")");
	gr.addColorStop(1, "rgb(255," + g + "," + b + ")");
	color.fillStyle = gr;
	color.fillRect(0, 4, 256, 24);
	gr = color.createLinearGradient(0, 0, 256, 0);
	gr.addColorStop(0, "rgb(" + r + ",0," + b + ")");
	gr.addColorStop(1, "rgb(" + r + ",255," + b + ")");
	color.fillStyle = gr;
	color.fillRect(0, 36, 256, 24);
	gr = color.createLinearGradient(0, 0, 256, 0);
	gr.addColorStop(0, "rgb(" + r + "," + g + ",0)");
	gr.addColorStop(1, "rgb(" + r + "," + g + ",255)");
	color.fillStyle = gr;
	color.fillRect(0, 68, 256, 24);
	
	c.fillStyle = "rgb(" + r + "," + g + "," + b + ")";
	c.fillRect(weftAreaX + leading, warpAreaY + leading, ballSize, ballSize);
}
// 色編集部のイベントハンドラー
var pressed = false;
function colorFixed(e)
{
	changeColor(e);
	pressed = false;
}
function changeColor0(e)
{
	pressed = true;
	changeColor(e);
}
function changeColor(e)
{
	if(pressed == false) return;
	
	var rect = e.target.getBoundingClientRect();
	var x =  Math.round(e.clientX - rect.left);
	var y =  Math.round(e.clientY - rect.top);
	var t;
	
	if(y < 32)
	{
		r = x;
		t = document.getElementById("r");
		t.value = r;
		drawColor();
		return;
	}
	if(y < 64)
	{
		g = x;
		t = document.getElementById("g");
		t.value = g;
		drawColor();
		return;
	}
	if(y < 96)
	{
		b = x;
		t = document.getElementById("b");
		t.value = b;
		drawColor();
		return;
	}
}

// 織り方の変更のハンドラー
function twillChange(e)
{
	var select;
	select = document.getElementById("twill");
	twillX = parseInt(select.value);
	twillY = parseInt(select.value);
	drawChecker();
}

// エンコーダーのヘルパー
function toHex(i)
{
	return String.fromCharCode((i >> 4) + 65, (i & 0x0F) + 65);
}
// デコーダーのヘルパー
function fromHex(h, i)
{
	return ((h.charCodeAt(i) - 65) << 4) + (h.charCodeAt(i+1) - 65);
}

// エンコードボタンのハンドラー
function onEncode(e)
{
	encode();
	
	var d = document.getElementById("data");
	d.value = encoded;

	var d = new Date();
	var y = d.getYear();
	var y1 = y  % 100 + 2001;
	d.setYear(y1);
	document.cookie = "CHECK=" + encoded + "; expires=" + d.toGMTString() + "; path=/checker/;";
}
// デコーダーのハンドラー
function onDecode()
{
	var d = document.getElementById("data");
	encoded = d.value;
	
	decode();
	
	draw();
}

// エンコードされたデータ
var encoded;
// エンコード本体
function encode()
{
	encoded = "";

	encoded += toHex(sz-1);
	encoded += toHex(sz-1);
	encoded += toHex(twillX);
	encoded += toHex(twillY);
	
	var radio;
	var flip = "0";
	radio = document.getElementById("fliph");
	if(radio.checked)
	{
		flip = "h";
	}
	radio = document.getElementById("flipv");
	if(radio.checked)
	{
		flip = "v";
	}
	radio = document.getElementById("flipw");
	if(radio.checked)
	{
		flip = "w";
	}
	encoded += flip;

	var render = "0A";
	radio = document.getElementById("0B");
	if(radio.checked)
	{
		render = "0B";
	}
	radio = document.getElementById("0C");
	if(radio.checked)
	{
		render = "0C";
	}
	radio = document.getElementById("1A");
	if(radio.checked)
	{
		render = "1A";
	}
	radio = document.getElementById("1B");
	if(radio.checked)
	{
		render = "1B";
	}
	radio = document.getElementById("1C");
	if(radio.checked)
	{
		render = "1C";
	}
	radio = document.getElementById("2A");
	if(radio.checked)
	{
		render = "2A";
	}
	radio = document.getElementById("2B");
	if(radio.checked)
	{
		render = "2B";
	}
	radio = document.getElementById("2C");
	if(radio.checked)
	{
		render = "2C";
	}
	encoded += render;
	
	var i;
	for(i = 0; i < sz; i++)
	{
		encoded += toHex(warp[i][0]);
		encoded += toHex(warp[i][1]);
		encoded += toHex(warp[i][2]);
		encoded += toHex(warp[i][3]);
	}
	for(i = 0; i < sz; i++)
	{
		encoded += toHex(weft[i][0]);
		encoded += toHex(weft[i][1]);
		encoded += toHex(weft[i][2]);
		encoded += toHex(weft[i][3]);
	}
}
// デコード本体
function decode()
{
	var szx = fromHex(encoded, 0);
	var szy = fromHex(encoded, 2);
	twillX = fromHex(encoded, 4);
	twillY = fromHex(encoded, 6);
	if(twillX == twillY)
	{
		var select = document.getElementById("twill");
		if(twillX == "1") { select.value="1";}
		if(twillY == "2") { select.value="2";}
		if(twillX == "4") { select.value="4";}
		if(twillY == "8") { select.value="8";}
	}
	
	var radio;
	radio = document.getElementById("flip" + encoded[8]);
	radio.checked = true;
	radio = document.getElementById(encoded[9] + encoded[10]);
	radio.checked = true;
	
	var j = 11;
	var i;
	for(i = 0; i < sz; i++)
	{
		warp[i][0] = fromHex(encoded, j); j++;j++;
		warp[i][1] = fromHex(encoded, j); j++;j++;
		warp[i][2] = fromHex(encoded, j); j++;j++;
		warp[i][3] = fromHex(encoded, j); j++;j++;
	}
	for(i = 0; i < sz; i++)
	{
		weft[i][0] = fromHex(encoded, j); j++;j++;
		weft[i][1] = fromHex(encoded, j); j++;j++;
		weft[i][2] = fromHex(encoded, j); j++;j++;
		weft[i][3] = fromHex(encoded, j); j++;j++;
	}
}

// 経糸緯糸の入れ替え
function swap()
{
	var v = warp;
	warp = weft;
	weft = v;
	draw();
}
// 経糸に緯糸のデータをコピー
function copydown()
{
	var i;
	for(i = 0; i < sz; i++)
	{
		warp[i][0] = weft[i][0];
		warp[i][1] = weft[i][1];
		warp[i][2] = weft[i][2];
		warp[i][3] = weft[i][3];
	}
	draw();
}
// 経糸のデータを横糸にコピー
function copyup()
{
	var i;
	for(i = 0; i < sz; i++)
	{
		weft[i][0] = warp[i][0];
		weft[i][1] = warp[i][1];
		weft[i][2] = warp[i][2];
		weft[i][3] = warp[i][3];
	}
	draw();
}

// 画像ファイル取得のAJAX
var request;
var cgi = "checker.cgi";

function onCheck()
{
	encode();
	
	var url = window.location.protocol + "/" + "/" + window.location.host + "/cgi-bin/" + cgi;
	request = new XMLHttpRequest();
	request.onreadystatechange = handler;
	request.open("POST", url, true);
	request.responseType = "blob";
	request.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	request.send("data=3" + encoded);
}

function handler()
{
	if(request.readyState == 4) {
		if(request.status == 200) {
			// 返されたデータを内部に保存して、それを img のソースとする
			var blob = new Blob([request.response], {type : "image/png"});
			document.getElementById("box").src = window.URL.createObjectURL(blob);
			document.getElementById("checked").style.display = "block";
		}
		request = null;
	}
}

function sampleあ()
{
	encoded  = "DPDPABAB00ABLCFKOPPBLCFKOPPBLCFKOPPBLCFKOPPBLCFKOPPBLCFKOPPBLCFKOPPBLCFKOPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPOPALAIPPOPALAIPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPBLCFKOPPBLCFKOPPBLCFKOPPBLCFKOPPBLCFKOPPBLCFKOPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPMLMIBMPPMLMIBMPPIHIHIHPPIHIHIHPPIHIHIHPPIHIHIHPPIHIHIHPPIHIHIHPPIHIHIHPPIHIHIHPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPBLCFKOPPBLCFKOPPBLCFKOPPBLCFKOPPBLCFKOPPBLCFKOPPBLCFKOPPBLCFKOPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPOPALAIPPOPALAIPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPBLCFKOPPBLCFKOPPBLCFKOPPBLCFKOPPBLCFKOPPBLCFKOPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPMLMIBMPPMLMIBMPPIHIHIHPPIHIHIHPPIHIHIHPPIHIHIHPPIHIHIHPPIHIHIHPPIHIHIHPPIHIHIHPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPPAFGFAJPP";
	
	decode();
	
	draw();
	
	onCheck();
}

function sample１()
{
	encoded  = "DPDPACACh0AMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPAAAAAAPPAAAAAAPPAAAAAAPPAAAAAAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMHAFAFPPMHAFAFPPMHAFAFPPMHAFAFPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPAAAAAAPPAAAAAAPPAAAAAAPPAAAAAAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPPMHAFAFPPMHAFAFPPMHAFAFPPMHAFAFPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPKLBGEAPPMAIAEAPPMAIAEAPPMAIAEAPPMAIAEAPP";
	
	decode();
	
	draw();
	
	onCheck();
}

function sampleＡ()
{
	encoded = "DPDPABABw1BOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPCGLOBFPPCGLOBFPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPAIAA@PPPAIAA@PPPAIAA@PPPAIAA@PPPPPPPPPPPPPPPPPPPLEAOAEPPLEAOAEPPLEAOAEPPLEAOAEPPLEAOAEPPLEAOAEPPLEAOAEPPLEAOAEPPLEAOAEPPLEAOAEPPAOAJOLPPAOAJOLPPAOAJOLPPAOAJOLPPAOAJOLPPAOAJOLPPAOAJOLPPAOAJOLPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPAAAAAAPPAAAAAAPPAAAAAAPPAAAAAAPPLEAOAEPPLEAOAEPPLEAOAEPPLEAOAEPPLEAOAEPPLEAOAEPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPCGLOBFPPCGLOBFPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPAIAA@PPPAIAA@PPPAIAA@PPPAIAA@PPPPPPPPPPPPPPPPPPPLEAOAEPPLEAOAEPPLEAOAEPPLEAOAEPPLEAOAEPPLEAOAEPPLEAOAEPPLEAOAEPPLEAOAEPPLEAOAEPPAOAJOLPPAOAJOLPPAOAJOLPPAOAJOLPPAOAJOLPPAOAJOLPPAOAJOLPPAOAJOLPPOHANCCPPOHANCCPPOHANCCPPOHANCCPPAAAAAAPPAAAAAAPPAAAAAAPPAAAAAAPPLEAOAEPPLEAOAEPPLEAOAEPPLEAOAEPPLEAOAEPPLEAOAEPP";
	
	decode();
	
	draw();
	
	onCheck();
}
