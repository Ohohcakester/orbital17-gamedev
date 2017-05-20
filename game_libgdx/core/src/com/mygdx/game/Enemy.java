package com.mygdx.game;

import com.badlogic.gdx.graphics.glutils.ShapeRenderer;
import com.badlogic.gdx.graphics.glutils.ShapeRenderer.ShapeType;

public class Enemy {
    GameVariables game = GameVariables.getInstance();

    float x, y, vx, vy;
    boolean isActive;
    int nextMovementCountdown;
    int hp;
    int shootCooldown;

    int width = 40;
    int height = 40;
    float speed = 2;

    Enemy(float x, float y) {
        this.x = x;
        this.y = y;
        isActive = true;
        nextMovementCountdown = 0;
        hp = 100;
        shootCooldown = 30;
    }

    void shootBullet() {
        float bulletVx = (game.random.nextInt(1000) - 500) * (2.5f / 500);
        game.bullets.add(new Bullet(x, y, bulletVx, -5, 0, 0.1f, 7, false));
    }

    void takeDamage(int damage) {
        hp -= damage;
        if (hp <= 0) remove();
    }

    void remove() {
        isActive = false;
    }

    void checkCollision() {
        for (Bullet bullet : game.bullets) {
            if (!bullet.isActive || !bullet.isPlayerBullet) continue;
            
            if (x - width/2 <= bullet.x + bullet.radius &&
                bullet.x - bullet.radius <= x + width/2 &&
                y - height/2 <= bullet.y + bullet.radius &&
                bullet.y - bullet.radius <= y + height/2) {

                bullet.remove();
                takeDamage(20);
            }
        }
    }

    void updateVelocity() {
        if (nextMovementCountdown > 0) {
            --nextMovementCountdown;
            return;
        } else {
            nextMovementCountdown = game.random.nextInt(120) + 60;
        }

        double angle = game.random.nextInt(1000) * (2*Math.PI/1000);
        vx = speed*(float)Math.cos(angle);
        vy = speed*(float)Math.sin(angle);

        if (x > game.RES_X && vx > 0) vx *= -1;
        if (x < 0 && vx < 0) vx *= -1;
        if (y > 2*game.RES_Y/3 && vy > 0) vy *= -1;
        if (y < game.RES_Y/4 && vy < 0) vy *= -1;
    }

    void updateShooting() {
        if (shootCooldown <= 0) {
            shootBullet();
            shootCooldown = 20;
        } else {
            --shootCooldown;
        }
    }

    void update() {
        updateVelocity();
        updateShooting();
        x += vx;
        y += vy;
        checkCollision();
    }

    void draw(ShapeRenderer shapeRenderer) {
        shapeRenderer.begin(ShapeType.Filled);
        shapeRenderer.setColor(1, 0, 0, 1);
        shapeRenderer.rect(x-width/2, y-height/2, width, height);
        shapeRenderer.end();
    }
}

