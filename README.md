Git Readme File


Things needing change:

player->updateVelocity is called in the manager::draw() because it draws the pressed keys to screen.  Should be called in manager::update()
