function main() {
    var foo = 'bar';
    
    console.log(foo);
    
    var entity = engine.createEntity();
    entity.loadTexture('pixel_art.bmp');
    entity.moveTo(700, 300);
    
    return foo;
};

main();

'foo';