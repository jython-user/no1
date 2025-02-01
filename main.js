let canvas = document.getElementById('myCanvas');
let ctx = canvas.getContext('2d');
let fps = document.getElementById('fps');
let imgData, then, now, wasm;
const size = 400;


fetch("main11.wasm")
.then(response => response.arrayBuffer())
.then(bytes => WebAssembly.instantiate(bytes, {}))
.then(results => {

    wasm = results.instance.exports;
    const memoryBuffer = new Uint8ClampedArray(wasm.memory.buffer);
    const offset = wasm.get_image_data();//wasm.c();
    // imgData to display it
    imgData = new ImageData(memoryBuffer.subarray(offset, offset + (size * size * 4)), size, size);

    wasm.init();
    animate();
    
}).catch((err) => {
    console.log(err)
})


document.addEventListener('keydown', (event) => {
    if (event.key === 'w') {
        wasm.forward();
    } else if (event.key === 's') {
        wasm.backward();
    }else if (event.key === 'ArrowUp') {
        wasm.up();
    } else if (event.key === 'ArrowDown') {
        wasm.down();
    } else if (event.key === 'ArrowRight') {
        wasm.right();
    } else if (event.key === 'ArrowLeft') {
        wasm.left();
    } 
});


function animate() {
    wasm.draw();
    //displays the imgData
    ctx.putImageData(imgData, 0, 0);
    now = performance.now();
    // prints the info for testing
    fps.textContent = wasm.print_y() + " - " + wasm.print_x() + " - " + wasm.print_z() + " | " + wasm.print_alpha() + " - " + wasm.print_beta() + " | " + ~~(1000 / (now - then));
    then = now;
    requestAnimationFrame(animate);
}

