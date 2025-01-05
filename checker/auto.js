// 画面ロード時のハンドラー
// ボタンにクリックハンドラーをセット
function setup()
{
	request = null;

	var now = new Date();
	var t = document.getElementById("v");
	t.value = now.getFullYear() + "年" + (now.getMonth() + 1) + "月" + now.getDate() + "日";

	var b;
	b = document.getElementById("acivivid");
	b.addEventListener("click", onGetImage);
	b = document.getElementById("acimild");
	b.addEventListener("click", onGetImage);
	b = document.getElementById("acisteady");
	b.addEventListener("click", onGetImage);
}

var request;
var url1;

// 画像取得ボタンのクリックハンドラー
// ボタンによって、URL とパラメーターの名前が違う
// AJAX でバイナリーデータを取得する
function onGetImage(e)
{
	if(request != null) return;

	var cgi = e.currentTarget.id + ".cgi";
	var param;
	if(cgi == "acivivid.cgi")
	{
		param = "?pg=";
	}
	if(cgi == "acimild.cgi")
	{
		param = "?sz=";
	}
	if(cgi == "acisteady.cgi")
	{
		param = "?jv=";
	}
	var name = document.getElementById("v").value;
	
	url1 = window.location.protocol + "/" + "/" + window.location.host + "/cgi-bin/" + cgi;
	url1 += param + encodeURIComponent(name);
	
	request = new XMLHttpRequest();
	request.onreadystatechange = handlerImage;
	request.responseType = "blob";
	request.open("GET", url1, true);
	request.send(null);
}

// 画像取得 AJAX のハンドラー
function handlerImage()
{
	if(request.readyState == 4) {
		if(request.status == 200) {
			// 返されたデータを内部に保存して、それを img のソースとする
			var blob = new Blob([request.response], {type : "image/png"});
			document.getElementById("box").src = window.URL.createObjectURL(blob);
		}
		request = null;
	}
}
