function main() {

    // Code
    var canvas = document.getElementById("PRJ");

    if (!canvas)
        console.log("Obtaining Canvas failed.\n");
    else
        console.log("Obtaining Canvas succeeded.\n");

    var context = canvas.getContext("2d");

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

function keyDown(event) {

    // code
    alert("A key is Pressed...");

}

function mouseDown() {
    // code
    alert("A mouse button is clicked...");
}
