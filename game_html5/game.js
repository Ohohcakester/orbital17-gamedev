// GLOBALS - START
var RES_X = 800;
var RES_Y = 600;
var frame = 0;

var key_left = false;
var key_right = false;
var key_space_click = false;
var key_z_click = false;

var player;
var bullets = [];
var enemies = [];
var platforms = [];
// GLOBALS - END

// Initialisation
var frameTime = 1000/60.0;
var dTime = 0;
var lastFrameTime = 0;

var mainCanvas = document.getElementById('mainCanvas');
var ctx = mainCanvas.getContext('2d');
mainCanvas.width = RES_X;
mainCanvas.height = RES_Y;
window.addEventListener('keydown', keyboardPress, false);
window.addEventListener('keyup', keyboardRelease, false);
// Initialisation - END


var Platform = function(x1, y1, x2, y2) {
    this.x1 = x1;
    this.y1 = y1;
    this.x2 = x2;
    this.y2 = y2;
};

Platform.prototype = {
    update: function() {

    },

    draw: function() {
        drawRect(this.x1, this.y1, this.x2-this.x1, this.y2-this.y1, '#00ff00');
    },
};

var Bullet = function(x, y, vx, vy, ax, ay, radius, isPlayerBullet) {
    this.x = x;
    this.y = y;
    this.vx = vx;
    this.vy = vy;
    this.ax = ax;
    this.ay = ay;
    this.radius = radius;
    this.isPlayerBullet = isPlayerBullet;
    
    this.isActive = true;
};

Bullet.prototype = {
    update: function() {
        this.vx += this.ax;
        this.vy += this.ay;
        this.x += this.vx;
        this.y += this.vy;

        if (this.isOffScreen()) {
            this.remove();
            return;
        }
    },

    remove: function() {
        this.isActive = false;
    },

    isOffScreen: function() {
        var tolerance = 100;
        return this.x < -this.radius - tolerance ||
                this.y < -this.radius - tolerance ||
                this.x > RES_X + this.radius + tolerance || 
                this.y > RES_Y + this.radius + tolerance;
    },

    draw: function() {
        drawCircle(this.x, this.y, this.radius, '#ffff00');
    },
};

var Player = function(x, y) {
    this.x = x;
    this.y = y;

    this.vx = 0;
    this.vy = 0;
    this.isOnGround = false;
    this.isFacingRight = true;
    this.isActive = true;

    this.width = 25;
    this.height = 40;
    this.speed = 3;
    this.gravity = 0.4;
    this.jumpSpeed = 10;
    this.hitboxRadius = 5;
};

Player.prototype = {
    remove: function() {
        this.isActive = false;
    },

    checkCollision: function() {
        for (var i=0; i<bullets.length; ++i) {
            var bullet = bullets[i];
            if (!bullet.isActive || bullet.isPlayerBullet) continue;
            
            var dx = bullet.x - this.x;
            var dy = bullet.y - this.y;
            var dr = this.hitboxRadius + bullet.radius;
            if (dx*dx + dy*dy <= dr*dr) {
                bullet.remove();
                this.remove();
            }
        }
    },

    physicsUpdate: function() {
        //x += vx;
        //y += vy;
        var newX = this.x + this.vx;
        var newY = this.y + this.vy;

        this.isOnGround = false;
        for (var i=0; i<platforms.length; ++i) {
            var platform = platforms[i];
            var x1 = platform.x1 - this.width/2;
            var x2 = platform.x2 + this.width/2;
            var y1 = platform.y1 - this.height/2;
            var y2 = platform.y2 + this.height/2;

            // temp variables so that I don't have to "this." everywhere
            var x = this.x;
            var y = this.y;
            var vx = this.vx;
            var vy = this.vy;

            // Collide with left wall, moving right.
            if (x <= x1 && x1 < x+vx) {
                if (vx*(y1-y) < (x1-x)*vy && (x1-x)*vy < vx*(y2-y)) {
                    newX = Math.min(x1, newX);
                    this.vx = 0;
                }
            }
            // Collide with right wall, moving left.
            if (x+vx < x2 && x2 <= x) {
                if (vx*(y2-y) < (x2-x)*vy && (x2-x)*vy < vx*(y1-y)) {
                    newX = Math.max(x2, newX);
                    this.vx = 0;
                }
            }

            // Collide with top wall, moving down. (Landing)
            if (y <= y1 && y1 < y+vy) {
                if (vy*(x1-x) <= (y1-y)*vx && (y1-y)*vx <= vy*(x2-x)) {
                    newY = Math.min(y1, newY);
                    this.vy = 0;

                    // Land
                    this.isOnGround = true;
                }
            }
            // Collide with bottom wall, moving up.
            if (y+vy < y2 && y2 <= y) {
                if (vy*(x2-x) <= (y2-y)*vx && (y2-y)*vx <= vy*(x1-x)) {
                    newY = Math.max(y2, newY);
                    this.vy = 0;
                }
            }
        }

        if (newX > RES_X - this.width/2) newX = RES_X - this.width/2;
        if (newX < this.width/2) newX = this.width/2;

        this.x = newX;
        this.y = newY;
    },

    update: function() {
        this.vx = 0;
        this.vy += this.gravity;
        if (key_left) this.vx -= this.speed;
        if (key_right) this.vx += this.speed;
        if (key_space_click) {
            if (this.isOnGround) this.vy = -this.jumpSpeed;
        }
        
        if (key_left && !key_right) this.isFacingRight = false;
        if (key_right && !key_left) this.isFacingRight = true;
        if (key_z_click) this.shootBullet();

        this.physicsUpdate();
        this.checkCollision();
    },

    shootBullet: function() {
        var bulletVx = 10;
        if (!this.isFacingRight) bulletVx *= -1;

        bullets.push(new Bullet(this.x, this.y, bulletVx, 0, 0, 0, 5, true));
    },

    draw: function() {
        drawRect(this.x - this.width/2, this.y - this.height/2, this.width, this.height, '#0000ff');
    },
};


var Enemy = function(x, y) {
    this.x = x;
    this.y = y;

    this.vx = 0;
    this.vy = 0;
    this.isActive = true;
    this.nextMovementCountdown = 0;
    this.hp = 100;
    this.shootCooldown = 30;

    this.width = 40;
    this.height = 40;
    this.speed = 2;
};

Enemy.prototype = {
    shootBullet: function() {
        var bulletVx = Math.random()*5.0 - 2.5;
        bullets.push(new Bullet(this.x, this.y, bulletVx, -5, 0, 0.1, 7, false));
    },

    takeDamage: function(damage) {
        this.hp -= damage;
        if (this.hp <= 0) this.remove();
    },

    remove: function() {
        this.isActive = false;
    },

    checkCollision: function() {
        for (var i=0; i<bullets.length; ++i) {
            var bullet = bullets[i];
            if (!bullet.isActive || !bullet.isPlayerBullet) continue;
            
            if (this.x - this.width/2 <= bullet.x + bullet.radius &&
                bullet.x - bullet.radius <= this.x + this.width/2 &&
                this.y - this.height/2 <= bullet.y + bullet.radius &&
                bullet.y - bullet.radius <= this.y + this.height/2) {

                bullet.remove();
                this.takeDamage(20);
            }
        }
    },

    updateVelocity: function() {
        if (this.nextMovementCountdown > 0) {
            --this.nextMovementCountdown;
            return;
        } else {
            this.nextMovementCountdown = Math.random()*120 + 60;
        }

        var angle = Math.random() * (2*3.1415926535);
        this.vx = this.speed*Math.cos(angle);
        this.vy = this.speed*Math.sin(angle);

        if (this.x > RES_X && this.vx > 0) this.vx *= -1;
        if (this.x < 0 && this.vx < 0) this.vx *= -1;
        if (this.y > 2*RES_Y/3 && this.vy > 0) this.vy *= -1;
        if (this.y < RES_Y/4 && this.vy < 0) this.vy *= -1;
    },

    updateShooting: function() {
        if (this.shootCooldown <= 0) {
            this.shootBullet();
            this.shootCooldown = 20;
        } else {
            --this.shootCooldown;
        }
    },

    update: function() {
        this.updateVelocity();
        this.updateShooting();
        this.x += this.vx;
        this.y += this.vy;
        this.checkCollision();
    },

    draw: function() {
        drawRect(this.x - this.width/2, this.y - this.height/2, this.width, this.height, '#ff0000');
    },
};



function initialiseGame() {
    bullets = [];
    enemies = [];
    platforms = [];

    player = new Player(RES_X/4, RES_Y/4);
    platforms.push(new Platform(120,310,220,330));
    platforms.push(new Platform(000,420,380,440));
    platforms.push(new Platform(430,360,550,380));
    platforms.push(new Platform(600,420,800,440));

    frame = 0;
}

function updateGame() {
    if (player.isActive) player.update();

    for (var i=0; i<bullets.length; ++i) {
        if (bullets[i].isActive) bullets[i].update();
    }

    for (var i=0; i<enemies.length; ++i) {
        if (enemies[i].isActive) enemies[i].update();
    }

    for (var i=0; i<platforms.length; ++i) {
        platforms[i].update();
    }

    if (frame%300 == 0) {
        enemies.push(new Enemy(Math.random()*RES_X, Math.random()*RES_Y/3));
    }

    resetKeyClicks();
    ++frame;
}

function drawGameFrame() {
    if (player.isActive) player.draw();

    for (var i=0; i<bullets.length; ++i) {
        if (bullets[i].isActive) bullets[i].draw();
    }

    for (var i=0; i<enemies.length; ++i) {
        if (enemies[i].isActive) enemies[i].draw();
    }

    for (var i=0; i<platforms.length; ++i) {
        platforms[i].draw();
    }
}

function keyboardPress(e) {
    console.log('Pressed ' + e.keyCode);
    switch(e.keyCode) {
        case 37: key_left = true; break;
        case 39: key_right = true; break;
        case 32: key_space_click = true; break;
        case 90: key_z_click = true; break;
        case 82: initialiseGame(); break;
        default: return;
    }
    e.preventDefault();
}

function keyboardRelease(e) {
    switch(e.keyCode) {
        case 37: key_left = false; break;
        case 39: key_right = false; break;
        default: return;
    }
    e.preventDefault();
}

function resetKeyClicks() {
    key_space_click = false;
    key_z_click = false;
}

// This is called in an infinite while loop.
function gameLoop(time){
    if (time !== undefined) {
        dTime += (time - lastFrameTime);
        lastFrameTime = time;
        
        // Safeguard (slowdown) to prevent game from lagging to death
        if (dTime > 5*frameTime) dTime = 5*frameTime;

        // Update Frame
        while (dTime > frameTime) {
            updateGame();
            dTime -= frameTime;
        }
        
        // Draw Frame
        clearScreen();
        drawGameFrame();
    }    
    window.requestAnimationFrame(gameLoop);
}

function main() {
    initialiseGame();
    gameLoop();
}