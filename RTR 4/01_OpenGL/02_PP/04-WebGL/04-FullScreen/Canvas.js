var canvas = null;
var bFullscreen = false;
var context = null;

function main() {
    // Code
    canvas = document.getElementById("PRJ");

    if (!canvas)
        console.log("Obtaining Canvas failed.\n");
    else
        console.log("Obtaining Canvas succeeded.\n");

    context = canvas.getContext("2d");

    if (!context)
        console.log("Obtaining Context failed.\n");
    else
        console.log("Obtaining Context succeeded.\n");

    context.fillStyle = "black";
    context.fillRect(0, 0, canvas.width, canvas.height);

    var str = "Hello world !!!";

    context.font = "48px sans-serif";

    context.fillStyle = "white";

    context.textAlign = "center";
    context.textBaseline = "middle";

    context.fillText(str, canvas.width / 2, canvas.height / 2);

    window.addEventListener("keydown", keyDown, false);
    window.addEventListener("click", mouseDown, false)
}

function toggleFullscreen() {

    // Code
    var fullscreen_element =
        document.fullscreenElement ||
        document.mozFullScreenElement ||
        document.webkitFullscreenElement ||
        document.msFullscreenElement ||
        null;

    if (fullscreen_element == null) {
        if (canvas.requestFullscreen)
            canvas.requestFullscreen();

        else if (canvas.mozRequestFullScreen)
            mozRequestFullScreen();

        else if (canvas.webkitRequestFullscreen)
            webkitRequestFullscreen();

        else if (canvas.msRequestFullscreen)
            msRequestFullscreen();

        bFullscreen = true;
    }

    else {
        if (document.exitFullscreen)
            document.exitFullscreen();

        else if (document.mozExitFullScreen)
            document.mozExitFullScreen();

        else if (document.webkitExitFullscreen)
            document.webkitExitFullscreen();

        else if (document.msExitFullscreen)
            document.msExitFullscreen();

        bFullscreen = false;
    }
}

function keyDown(event) {

    // Code
    switch (event.keyCode) {
        case 70:
            toggleFullscreen();
            break;
    }
}

function mouseDown() {

    // Code
}