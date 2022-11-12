function main() {

    // code
    var canvas = document.getElementById("PRJ");
    if (!canvas) {
        console.log("Obtaining Canvas failed\n");
    } else {
        console.log("Obtaining Canvas Suceeded\n");
    }

    var context = canvas.getContext("2d");
    if (!canvas) {
        console.log("Obtaining context failed\n");
    } else {
        console.log("Obtaining context Suceeded\n");
    }

    context.fillStyle = "black";
    context.fillRect(0, 0, canvas.clientWidth, canvas.clientHeight);

}
