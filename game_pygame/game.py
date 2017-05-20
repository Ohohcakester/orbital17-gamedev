import sys, pygame
import random
import math
FRAME_UPDATE_EVENT = pygame.USEREVENT+1

# GLOBALS - START
screen = None
RES_X = 800
RES_Y = 600
frame = 0

key_left = False
key_right = False
key_space_click = False
key_z_click = False

player = None
bullets = []
enemies = []
platforms = []
# GLOBALS - END

class Platform(object):
    def __init__(self, x1, y1, x2, y2):
        self.x1 = x1
        self.y1 = y1
        self.x2 = x2
        self.y2 = y2

    def update(self):
        pass

    def draw(self):
        pygame.draw.rect(screen, (0,255,0),
            (self.x1, self.y1, self.x2-self.x1, self.y2-self.y1))

class Bullet(object):
    def __init__(self, x, y, vx, vy, ax, ay, radius, isPlayerBullet):
        self.x = x
        self.y = y
        self.vx = vx
        self.vy = vy
        self.ax = ax
        self.ay = ay
        self.radius = radius
        self.isPlayerBullet = isPlayerBullet
        
        self.isActive = True

    def update(self):
        self.vx += self.ax
        self.vy += self.ay
        self.x += self.vx
        self.y += self.vy

        if self.isOffScreen():
            self.remove()
            return

    def remove(self):
        self.isActive = False

    def isOffScreen(self):
        tolerance = 100
        return self.x < -self.radius - tolerance or \
                self.y < -self.radius - tolerance or \
                self.x > RES_X + self.radius + tolerance or  \
                self.y > RES_Y + self.radius + tolerance

    def draw(self):
        pygame.draw.circle(screen, (255,255,0), (int(self.x), int(self.y)), self.radius)

class Player(object):
    def __init__(self, x, y):
        self.x = x
        self.y = y

        self.vx = 0
        self.vy = 0
        self.isOnGround = False
        self.isFacingRight = True
        self.isActive = True

        self.width = 25
        self.height = 40
        self.speed = 3
        self.gravity = 0.4
        self.jumpSpeed = 10
        self.hitboxRadius = 5

    def remove(self):
        self.isActive = False

    def checkCollision(self):
        for bullet in bullets:
            if not bullet.isActive or bullet.isPlayerBullet: continue
            
            dx = bullet.x - self.x
            dy = bullet.y - self.y
            dr = self.hitboxRadius + bullet.radius
            if dx*dx + dy*dy <= dr*dr:
                bullet.remove()
                self.remove()

    def physicsUpdate(self):
        # x += vx
        # y += vy
        newX = self.x + self.vx
        newY = self.y + self.vy

        self.isOnGround = False
        for platform in platforms:
            x1 = platform.x1 - self.width/2
            x2 = platform.x2 + self.width/2
            y1 = platform.y1 - self.height/2
            y2 = platform.y2 + self.height/2

            # temp variables so that I don't have to "self." everywhere.
            x = self.x
            y = self.y
            vx = self.vx
            vy = self.vy

            # Collide with left wall, moving right.
            if x <= x1 and x1 < x+vx:
                if vx*(y1-y) < (x1-x)*vy and (x1-x)*vy < vx*(y2-y):
                    newX = min(x1, newX)
                    self.vx = 0
            # Collide with right wall, moving left.
            if x+vx < x2 and x2 <= x:
                if vx*(y2-y) < (x2-x)*vy and (x2-x)*vy < vx*(y1-y):
                    newX = max(x2, newX)
                    self.vx = 0

            # Collide with top wall, moving down. (Landing)
            if y <= y1 and y1 < y+vy:
                if vy*(x1-x) <= (y1-y)*vx and (y1-y)*vx <= vy*(x2-x):
                    newY = min(y1, newY)
                    self.vy = 0

                    # Land
                    self.isOnGround = True
            # Collide with bottom wall, moving up.
            if y+vy < y2 and y2 <= y:
                if vy*(x2-x) <= (y2-y)*vx and (y2-y)*vx <= vy*(x1-x):
                    newY = max(y2, newY)
                    self.vy = 0

        if newX > RES_X - self.width/2: newX = RES_X - self.width/2
        if newX < self.width/2: newX = self.width/2

        self.x = newX
        self.y = newY

    def update(self):
        self.vx = 0
        self.vy += self.gravity
        if key_left: self.vx -= self.speed
        if key_right: self.vx += self.speed
        if key_space_click:
            if self.isOnGround: self.vy = -self.jumpSpeed
        
        if key_left and not key_right: self.isFacingRight = False
        if key_right and not key_left: self.isFacingRight = True
        if key_z_click: self.shootBullet()

        self.physicsUpdate()
        self.checkCollision()

    def shootBullet(self):
        bulletVx = 10
        if not self.isFacingRight: bulletVx *= -1

        bullets.append(Bullet(self.x, self.y, bulletVx, 0, 0, 0, 5, True))

    def draw(self):
        pygame.draw.rect(screen, (0,0,255),
            (self.x - self.width/2, self.y - self.height/2, self.width, self.height))


class Enemy(object):
    def __init__(self, x, y):
        self.x = x
        self.y = y

        self.vx = 0
        self.vy = 0
        self.isActive = True
        self.nextMovementCountdown = 0
        self.hp = 100
        self.shootCooldown = 30

        self.width = 40
        self.height = 40
        self.speed = 2

    def shootBullet(self):
        bulletVx = random.random()*5.0 - 2.5
        bullets.append(Bullet(self.x, self.y, bulletVx, -5, 0, 0.1, 7, False))

    def takeDamage(self, damage):
        self.hp -= damage
        if self.hp <= 0: self.remove()

    def remove(self):
        self.isActive = False

    def checkCollision(self):
        for bullet in bullets:
            if not bullet.isActive or not bullet.isPlayerBullet: continue
            
            if self.x - self.width/2 <= bullet.x + bullet.radius and \
                bullet.x - bullet.radius <= self.x + self.width/2 and \
                self.y - self.height/2 <= bullet.y + bullet.radius and \
                bullet.y - bullet.radius <= self.y + self.height/2:

                bullet.remove()
                self.takeDamage(20)

    def updateVelocity(self):
        if self.nextMovementCountdown > 0:
            self.nextMovementCountdown -= 1
            return
        else:
            self.nextMovementCountdown = random.randrange(60,180)

        angle = random.random() * (2*math.pi)
        self.vx = self.speed*math.cos(angle)
        self.vy = self.speed*math.sin(angle)

        if self.x > RES_X and self.vx > 0: self.vx *= -1
        if self.x < 0 and self.vx < 0: self.vx *= -1
        if self.y > 2*RES_Y/3 and self.vy > 0: self.vy *= -1
        if self.y < RES_Y/4 and self.vy < 0: self.vy *= -1

    def updateShooting(self):
        if self.shootCooldown <= 0:
            self.shootBullet()
            self.shootCooldown = 20
        else:
            self.shootCooldown -= 1

    def update(self):
        self.updateVelocity()
        self.updateShooting()
        self.x += self.vx
        self.y += self.vy
        self.checkCollision()

    def draw(self):
        pygame.draw.rect(screen, (255,0,0),
            (self.x - self.width/2, self.y - self.height/2, self.width, self.height))


def initialiseGame():
    global bullets, enemies, platforms, player, frame
    bullets = []
    enemies = []
    platforms = []

    player = Player(RES_X/4, RES_Y/4)
    platforms.append(Platform(120,310,220,330))
    platforms.append(Platform(000,420,380,440))
    platforms.append(Platform(430,360,550,380))
    platforms.append(Platform(600,420,800,440))

    frame = 0

def updateGame():
    global frame

    if player.isActive: player.update()

    for bullet in bullets:
        if bullet.isActive: bullet.update()

    for enemy in enemies:
        if enemy.isActive: enemy.update()

    for platform in platforms:
        platform.update()

    if frame%300 == 0:
        enemies.append(Enemy(random.randrange(0,RES_X), random.randrange(0,RES_Y//3)))

    resetKeyClicks()
    frame += 1

def drawGameFrame():
    if player.isActive: player.draw()

    for bullet in bullets:
        if bullet.isActive: bullet.draw()

    for enemy in enemies:
        if enemy.isActive: enemy.draw()

    for platform in platforms:
        platform.draw()

def keyDown(key):
    global key_left, key_right, key_space_click, key_z_click
    if key == pygame.K_LEFT: key_left = True
    elif key == pygame.K_RIGHT: key_right = True
    elif key == pygame.K_SPACE: key_space_click = True
    elif key == pygame.K_z: key_z_click = True
    elif key == pygame.K_r: initialiseGame()

def keyUp(key):
    global key_left, key_right
    if key == pygame.K_LEFT: key_left = False
    elif key == pygame.K_RIGHT: key_right = False

def resetKeyClicks():
    global key_space_click, key_z_click
    key_space_click = False
    key_z_click = False

def processEvent(event):
    if event.type == FRAME_UPDATE_EVENT:
        updateGame()
    if event.type == pygame.QUIT:
        sys.exit()
    elif event.type == pygame.KEYDOWN:
        keyDown(event.key)
    elif event.type == pygame.KEYUP:
        keyUp(event.key)

def main():
    pygame.init()
    global screen
    screen = pygame.display.set_mode((RES_X, RES_Y))
    pygame.display.set_caption('This is not a game. This is serious business.')
    
    frameTime = 1000/60 # in ms
    dTime = 0.0
    lastFrameTicks = pygame.time.get_ticks()
    
    initialiseGame()
    while True:
        currentFrameTicks = pygame.time.get_ticks()
        dTime += (currentFrameTicks - lastFrameTicks)
        lastFrameTicks = currentFrameTicks
        
        for event in pygame.event.get():
            processEvent(event)
        
        # Safeguard (slowdown) to prevent game from lagging to death
        if dTime > 5*frameTime: dTime = 5*frameTime
        
        # Update Frame
        while dTime > frameTime:
            dTime -= frameTime
            updateGame()
        
        # Draw Frame
        screen.fill((0,0,0))
        drawGameFrame()
        pygame.display.flip()

if __name__ == '__main__':
    main()