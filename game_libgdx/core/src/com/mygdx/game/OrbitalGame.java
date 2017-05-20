package com.mygdx.game;

import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.graphics.glutils.ShapeRenderer;


import java.util.ArrayList;

public class OrbitalGame extends ApplicationAdapter {
	OrthographicCamera camera;
	ShapeRenderer shapeRenderer;
	GameVariables game;

	long lastFrameTime;
	double dTime;
	double frameTime = 1000/60.0; // in ms
	
	@Override
	public void create() {
		int RES_X = Gdx.graphics.getWidth();
		int RES_Y = Gdx.graphics.getHeight();

		// Change to y-down instead of y-up
		camera = new OrthographicCamera(RES_X, RES_Y);
		camera.setToOrtho(true, RES_X, RES_Y);

		// Shape Renderer to draw primitive 2D shapes
		shapeRenderer = new ShapeRenderer();
		shapeRenderer.setProjectionMatrix(camera.combined);

		// Initialise GameVariables
		game = new GameVariables(RES_X, RES_Y);
        Gdx.input.setInputProcessor(game);

        lastFrameTime = System.currentTimeMillis();

        // Initialise game
		game.initialiseGame();
	}

	@Override
	public void render() {
		long currentFrameTime = System.currentTimeMillis();
		dTime += (currentFrameTime - lastFrameTime);
		lastFrameTime = currentFrameTime;

        // Safeguard (slowdown) to prevent game from lagging to death
        if (dTime > 5*frameTime) dTime = 5*frameTime;

		// Update game
		while (dTime > frameTime) {
			dTime -= frameTime;
			game.updateGame();
		}

		// Draw frame
		Gdx.gl.glClearColor(0, 0, 0, 1);
		Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT);
		game.drawGameFrame(shapeRenderer);
	}
	
	@Override
	public void dispose() {
		shapeRenderer.dispose();
	}
}
