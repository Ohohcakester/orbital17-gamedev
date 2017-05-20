function drawRect(x, y, width, height, colour) {
    ctx.fillStyle = colour;
    ctx.beginPath();
    ctx.rect(x,y,width,height);
    ctx.closePath();
    ctx.fill();
}

function drawCircle(cx, cy, radius, colour) {
    ctx.fillStyle = colour;
    ctx.beginPath();
    ctx.arc(cx,cy, radius, 0, 2*Math.PI, true);
    ctx.closePath();
    ctx.fill();
}

function drawPolygon(cx, cy, points, colour) {
    ctx.fillStyle = colour;
    ctx.beginPath();
    ctx.moveTo(points[0]+cx, points[1]+cy);
    for(var i=2; i<points.length; i+=2){
        ctx.lineTo(points[i]+cx, points[i+1]+cy)
    }
    ctx.closePath();
    ctx.fill();
}

function drawText(text, size, x, y, colour) {
    ctx.fillStyle = colour;
    ctx.font = size+'px Georgia';
    ctx.fillText(text, x, y);
}

function clearScreen() {
    ctx.fillStyle = "#000000"
    ctx.clearRect(0,0,mainCanvas.width,mainCanvas.height);
    ctx.fillRect(0,0,mainCanvas.width,mainCanvas.height);
}
