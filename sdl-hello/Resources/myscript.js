function main() {
    var foo = 'bar';
    
    console.log(foo);
    
    var entity = engine.createEntity();
    entity.loadTexture('hello.bmp');
    entity.moveTo(700, 300);
    
    var entityTwo = engine.createEntity();
    entityTwo.loadTexture('pixel_art.bmp');
    entityTwo.moveTo(800, 50);
    
    engine.onMouseDown(function (x, y) {
        entityTwo.moveTo(x, y);
    });
    
    return foo;
};

main();

'foo';