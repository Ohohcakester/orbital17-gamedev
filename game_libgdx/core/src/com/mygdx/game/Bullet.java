package com.mygdx.game;

import com.badlogic.gdx.graphics.glutils.ShapeRenderer;
import com.badlogic.gdx.graphics.glutils.ShapeRenderer.ShapeType;

public class Bullet {
    GameVariables game = GameVariables.getInstance();

    float x, y, vx, vy, ax, ay;
    boolean isActive;
    boolean isPlayerBullet;
    float radius;

    Bullet(float x, float y, float vx, float vy, float ax, float ay, float radius, boolean isPlayerBullet) {
        this.x = x;
        this.y = y;
        this.vx = vx;
        this.vy = vy;
        this.ax = ax;
        this.ay = ay;
        this.radius = radius;
        this.isPlayerBullet = isPlayerBullet;

        isActive = true;
    }

    void update() {
        vx += ax;
        vy += ay;
        x += vx;
        y += vy;

        if (isOffScreen()) {
            remove();
            return;
        }
    }

    void remove() {
        isActive = false;
    }

    boolean isOffScreen() {
        float tolerance = 100.f;
        return x < -radius - tolerance ||
                y < -radius - tolerance ||
                x > game.RES_X + radius + tolerance || 
                y > game.RES_Y + radius + tolerance;
    }

    void draw(ShapeRenderer shapeRenderer) {
        shapeRenderer.begin(ShapeType.Filled);
        shapeRenderer.setColor(1, 1, 0, 1);
        shapeRenderer.circle(x, y, radius);
        shapeRenderer.end();
    }
}
