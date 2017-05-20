package com.mygdx.game;

import com.badlogic.gdx.Input;
import com.badlogic.gdx.InputProcessor;
import com.badlogic.gdx.graphics.glutils.ShapeRenderer;

import java.util.Random;
import java.util.ArrayList;

public class GameVariables implements InputProcessor {

    public final Random random;
    public final int RES_X;
    public final int RES_Y;

    public int frame = 0;

    public boolean key_space_click = false;
    public boolean key_z_click = false;

    public Player player;
    public ArrayList<Bullet> bullets;
    public ArrayList<Enemy> enemies;
    public ArrayList<Platform> platforms;

    private static GameVariables instance;
    public static GameVariables getInstance() {
        return instance;
    }

    public GameVariables(int RES_X, int RES_Y) {
        this.RES_X = RES_X;
        this.RES_Y = RES_Y;
        this.random = new Random();

        if (GameVariables.instance != null) {
            throw new UnsupportedOperationException("Trying to create more than one instance of singleton!");
        }
        GameVariables.instance = this;
    }

    void initialiseGame() {
        bullets = new ArrayList<Bullet>();
        enemies = new ArrayList<Enemy>();
        platforms = new ArrayList<Platform>();

        player = new Player(RES_X/4, RES_Y/4);
        platforms.add(new Platform(120,310,220,330));
        platforms.add(new Platform(000,420,380,440));
        platforms.add(new Platform(430,360,550,380));
        platforms.add(new Platform(600,420,800,440));

        frame = 0;
    }

    void updateGame() {
        if (player.isActive) player.update();

        for (Bullet bullet : bullets) {
            if (bullet.isActive) bullet.update();
        }

        for (Enemy enemy : enemies) {
            if (enemy.isActive) enemy.update();
        }

        for (Platform platform : platforms) {
            platform.update();
        }

        if (frame%300 == 0) {
            enemies.add(new Enemy(random.nextInt(RES_X), random.nextInt(RES_Y/3)));
        }

        resetKeyClicks();
        ++frame;
    }

    void drawGameFrame(ShapeRenderer shapeRenderer) {
        if (player.isActive) player.draw(shapeRenderer);

        for (Bullet bullet : bullets) {
            if (bullet.isActive) bullet.draw(shapeRenderer);
        }

        for (Enemy enemy : enemies) {
            if (enemy.isActive) enemy.draw(shapeRenderer);
        }

        for (Platform platform : platforms) {
            platform.draw(shapeRenderer);
        }
    }

    void resetKeyClicks() {
        key_space_click = false;
        key_z_click = false;
    }

    @Override
    public boolean keyDown(int keyCode) {
        switch(keyCode) {
            case Input.Keys.SPACE: key_space_click = true; break;
            case Input.Keys.Z: key_z_click = true; break;
            case Input.Keys.R: initialiseGame(); break;
        }
        return true;
    }

    // All the other interface members that I don't need to implement
    @Override
    public boolean keyUp(int keycode) {
        return false;
    }

    @Override
    public boolean keyTyped(char character) {
        return false;
    }

    @Override
    public boolean touchDown(int screenX, int screenY, int pointer, int button) {
        return false;
    }

    @Override
    public boolean touchUp(int screenX, int screenY, int pointer, int button) {
        return false;
    }

    @Override
    public boolean touchDragged(int screenX, int screenY, int pointer) {
        return false;
    }

    @Override
    public boolean mouseMoved(int screenX, int screenY) {
        return false;
    }

    @Override
    public boolean scrolled(int amount) {
        return false;
    }
}