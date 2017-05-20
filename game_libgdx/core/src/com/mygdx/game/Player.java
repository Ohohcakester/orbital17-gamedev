package com.mygdx.game;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.Input;
import com.badlogic.gdx.graphics.glutils.ShapeRenderer;
import com.badlogic.gdx.graphics.glutils.ShapeRenderer.ShapeType;

public class Player {
    GameVariables game = GameVariables.getInstance();

    float x, y;
    float vx, vy;
    boolean isOnGround;
    boolean isFacingRight;
    boolean isActive;

    int width = 25;
    int height = 40;
    int speed = 3;
    float gravity = 0.4f;
    float jumpSpeed = 10;
    float hitboxRadius = 5;

    Player(float x, float y) {
        this.x = x;
        this.y = y;
        vx = 0;
        vy = 0;
        isOnGround = false;
        isFacingRight = true;
        isActive = true;
    }

    void remove() {
        isActive = false;
    }

    void checkCollision() {
        for (Bullet bullet : game.bullets) {
            if (!bullet.isActive || bullet.isPlayerBullet) continue;
            
            float dx = bullet.x - x;
            float dy = bullet.y - y;
            float dr = hitboxRadius + bullet.radius;
            if (dx*dx + dy*dy <= dr*dr) {
                bullet.remove();
                remove();
            }
        }
    }

    void physicsUpdate() {
        //x += vx;
        //y += vy;
        float newX = x + vx;
        float newY = y + vy;

        isOnGround = false;
        for (Platform platform : game.platforms) {
            float x1 = platform.x1 - width/2;
            float x2 = platform.x2 + width/2;
            float y1 = platform.y1 - height/2;
            float y2 = platform.y2 + height/2;

            // Collide with left wall, moving right.
            if (x <= x1 && x1 < x+vx) {
                if (vx*(y1-y) < (x1-x)*vy && (x1-x)*vy < vx*(y2-y)) {
                    newX = Math.min(x1, newX);
                    vx = 0;
                }
            }
            // Collide with right wall, moving left.
            if (x+vx < x2 && x2 <= x) {
                if (vx*(y2-y) < (x2-x)*vy && (x2-x)*vy < vx*(y1-y)) {
                    newX = Math.max(x2, newX);
                    vx = 0;
                }
            }

            // Collide with top wall, moving down. (Landing)
            if (y <= y1 && y1 < y+vy) {
                if (vy*(x1-x) <= (y1-y)*vx && (y1-y)*vx <= vy*(x2-x)) {
                    newY = Math.min(y1, newY);
                    vy = 0;

                    // Land
                    isOnGround = true;
                }
            }
            // Collide with bottom wall, moving up.
            if (y+vy < y2 && y2 <= y) {
                if (vy*(x2-x) <= (y2-y)*vx && (y2-y)*vx <= vy*(x1-x)) {
                    newY = Math.max(y2, newY);
                    vy = 0;
                }
            }
        }

        if (newX > game.RES_X - width/2) newX = game.RES_X - width/2;
        if (newX < width/2) newX = width/2;

        x = newX;
        y = newY;
    }

    void update() {
        vx = 0;
        vy += gravity;

        boolean key_left = Gdx.input.isKeyPressed(Input.Keys.LEFT);
        boolean key_right = Gdx.input.isKeyPressed(Input.Keys.RIGHT);

        if (key_left) vx -= speed;
        if (key_right) vx += speed;
        if (game.key_space_click) {
            if (isOnGround) vy = -jumpSpeed;
        }
        
        if (key_left && !key_right) isFacingRight = false;
        if (key_right && !key_left) isFacingRight = true;
        if (game.key_z_click) shootBullet();

        physicsUpdate();
        checkCollision();
    }

    void shootBullet() {
        float bulletVx = 10;
        if (!isFacingRight) bulletVx *= -1;

        game.bullets.add(new Bullet(x, y, bulletVx, 0, 0, 0, 5, true));
    }

    void draw(ShapeRenderer shapeRenderer) {
        shapeRenderer.begin(ShapeType.Filled);
        shapeRenderer.setColor(0, 0, 1, 1);
        shapeRenderer.rect(x-width/2, y-height/2, width, height);
        shapeRenderer.end();
    }
}
