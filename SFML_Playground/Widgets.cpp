#pragma once
#include "Widgets.h"


// W_MainMenu -------------------------------------------------------------------------------------

void W_MainMenu::construct()
{
	menu_highscore->setText("Highscore: " + std::to_string(SaveGame::Stored_Save));
}

W_MainMenu::W_MainMenu() : InputWidget()
{
	const std::vector<ButtonConstruct> MAIN_MENU_CONSTR = {
		{windowCenter + sf::Vector2f{ 0, -300 },    sf::Vector2f{ 350, 120 }, sf::Color::Transparent,   100, "ARENA",											sf::Color::White},
		{windowCenter + sf::Vector2f{ 0, -200 },    sf::Vector2f{ 100, 100 }, sf::Color::Transparent,   16, "Higscore: " + std::to_string(SaveGame::Stored_Save),	sf::Color::White},
		{windowCenter + sf::Vector2f{ 0, 0 },       sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "START",													sf::Color::Black},
		{windowCenter + sf::Vector2f{ 0, 150 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "OPTIONS",													sf::Color::Black},
		{windowCenter + sf::Vector2f{ 0, 300 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "QUIT",														sf::Color::Black}
	};

	menu_title = new Button(MAIN_MENU_CONSTR[0]);
	menu_highscore = new Button(MAIN_MENU_CONSTR[1]);
	menu_startButton = new Button(MAIN_MENU_CONSTR[2]);
	menu_optionsButton = new Button(MAIN_MENU_CONSTR[3]);
	menu_quitButton = new Button(MAIN_MENU_CONSTR[4]);
	shapes = { menu_title, menu_highscore, menu_startButton, menu_optionsButton, menu_quitButton };
}
bool W_MainMenu::isMouseOver()
{
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
	if (menu_title->isMouseOver(mousePos))
	{
		bool alreadyChanged = (menu_title->getColor(true) == sf::Color::Red);
		menu_title->setColor((alreadyChanged ? sf::Color::White : sf::Color::Red), true);
		return true;
	}
	if (menu_startButton->isMouseOver(mousePos))
	{
		gameInstance.setGameState(GAME_LAUNCHING);
		return true;
	}
	if (menu_quitButton->isMouseOver(mousePos))
	{
		gameInstance.setGameState(GAME_ENDED);
		return true;
	}
	// On no button-mouse overlap
	return false;
}

sf::Keyboard::Key W_MainMenu::keyboardInput(sf::Event* eventRef)
{
	// Call the parent function and store the result
	sf::Keyboard::Key keyPressed = InputWidget::keyboardInput(eventRef);
	switch (keyPressed)
	{
	case sf::Keyboard::Escape:
		window->close();
		break;
	default:
		break;
	}

	return keyPressed;
}

// W_Gameplay -------------------------------------------------------------------------------------

W_Gameplay::W_Gameplay() : InputWidget()
{
	

	targetController = new TargetController();
	healthBar = new Timer(10.0f, windowSize.x, 100.0f, sf::Vector2f(windowCenter.x, 0.0f));
	shapes = { targetController, &flashlightMask, healthBar };
}

void W_Gameplay::construct()
{
	if (gameInstance.getGameState() == GAME_LAUNCHING)
	{
		// Reset values to game start values
		hitTargets = 0;
		targetController->initSpawner(*window);
		healthBar->setMaxTime(startTimer, true);
		// Add Gameplay objects to shapes vector to draw them
		gameInstance.setGameState(IN_GAME);
	}
}

void W_Gameplay::update(const float& deltaTime)
{
	WidgetMenu::update(deltaTime);
	flashlightMask.update(deltaTime);

	if (gameInstance.getGameState() >= GAME_LAUNCHING)
	{
		// Update Gameplay objects with respectable params
		healthBar->update(deltaTime);
		targetController->update(*window);
		if (healthBar->isFinished())
		{
			// Remove all shapes from vector for menu shapes
			if (hitTargets > SaveGame::Stored_Save) SaveGame::Stored_Save = hitTargets; // Update highscore value if new value is bigger
			SaveGame::saveData(); // Save highscore value (didn't change if no greater was achieved)
			// switch back to MainMenu
			gameInstance.setGameState(MENU_SCREEN);
		}
	}
	for (sf::Drawable* elem : shapes) flashlightMask.drawOtherScene(elem);
}

sf::Keyboard::Key W_Gameplay::keyboardInput(sf::Event* eventRef)
{
	// Call the parent function and store the result
	sf::Keyboard::Key keyPressed = InputWidget::keyboardInput(eventRef);
	switch (keyPressed)
	{
	case sf::Keyboard::Escape:
		// on Esc go to MainMenu
		gameInstance.setGameState(MENU_SCREEN);
		break;
	default:
		break;
	}

	return keyPressed;
}

bool W_Gameplay::isMouseOver()
{
	flashlightMask.resetRadius();
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
	if (targetController->clickedAny(mousePos))
	{
		// Increase targetsHit and change HealthBar accoridngly
		hitTargets++;
		float newMaxTime = startTimer - (float(int(hitTargets) / 3) * 0.2f);
		healthBar->setMaxTime(std::max(newMaxTime, minTimer)); // Shorten HealthBar lifespan
		healthBar->setCurrentTime(healthBar->getCurrentTime() + (healthBar->getMaxTime() / 5.0f)); // Regen so a fifth of the max lifespan
		return true;
	}
	return false;
}

void W_Gameplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto elem : shapes) target.draw(*elem, states);

	return;
}