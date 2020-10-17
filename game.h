#ifndef GAME_H
#define GAME_H


#include <QPainter>
#include <QTime>
#include <QIcon>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QSound>
#include <iostream>
#include <random>
#include <QWidget>
#include <QKeyEvent>
#include <vector>

class coordinate
{
 public:
    int m_x;
    int m_y;

    coordinate(int x=0, int y=0) : m_x(x), m_y(y) {}

    //copy constructor
    coordinate(const coordinate &other) {
        m_x = other.m_x;
        m_y = other.m_y;
    }

    //assignment overload operator
    coordinate& operator=(const coordinate &coord) {
        m_x = coord.m_x;
        m_y = coord.m_y;
        return *this;
    }

    bool operator==(const coordinate& rhs) {
        return this->m_x == rhs.m_x && this->m_y == rhs.m_y;
    }


};

class controller;


class view : public QWidget
{
private:

    controller *c;
    QWindow window(QWidget);

    QImage dot_png;
    QImage head_png;
    QImage apple_png;
    QImage obstacle_png;
    //play area width
    static const int B_WIDTH = 500;
    //play area height
    static const int B_HEIGHT = 500;
    //delay offset for the speed game/refresh
    const int DELAY = 100;

public:

    view(QWidget *parent = 0);
    view(controller* c);// : c(c) { std::cout << "view constructor" << std::endl; }
    void loadImages();
    void initGame();
    int getTimerId();
    void doDrawing();
    void gameOver(QPainter &);
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
    void keyPressEvent(QKeyEvent *);
    int timerId;

};


enum class Movement{ north, south, east, west };

class model
{
private:
    controller *c;

    // store obstacles and check for duplicity
    std::vector<coordinate> placed_obstacles;
    //vevtor for snake coordinates
    std::vector<coordinate> the_snake;


    //this variable is needed otherwise snake is drawn on top of itself
    const int BODY_SIZE = 10;
    //int timerId;
    int snake_size = 3;//initial size of the snake
    coordinate apple;
    coordinate snake;

    //delay offset for the speed game/refresh
    const int DELAY = 100;
    int score = 0;
    bool GamePlay;

    //play area width
    static const int B_WIDTH = 500;
    //play area height
    static const int B_HEIGHT = 500;


    //Turn change;
    Movement direction;
public:
    model();
    model(controller* c) : c(c) {GamePlay = true; direction = Movement::west; }
    void initGame();

    int getScore();
    void placeFood();
    void placeObstacle();
    void grow(int amount);
    void foodCollision();
    void obstacleCollision();
    void checkCollision();
    void move();
    bool GetGamePlay();
    Movement getDir();
    void setDirection(Movement variable) { direction = variable; }
    coordinate& getApple();
    coordinate& getSnake();
    std::vector<coordinate>& getPlaced();
    std::vector<coordinate>& getSnakeVec();
};


class controller
{
private:
    model* m;
    view* v;
public:
    controller() : m(new model(this)), v(new view(this)) {
        v->show();
    }


    void CinitGame();

    int CgetTimer();
    void CKillTimer();
    void CdoDrawing();

    bool CgamePlay();
    int CgetScore();

    void CFoodCollision();
    void CobstacleCollison();
    void CcheckCollision();
    void Cmove();
    void CpressEvent(QKeyEvent *e);
    Movement CgetDir();
    void CsetDir(Movement variable);

    coordinate& CgetApple();
    coordinate& CgetSnake();
    std::vector<coordinate>& CgetPlaced();
    std::vector<coordinate>& CgetSnakeVec();
};

/*--------------------------------------------------------------------*/

//place all functions for all classes below here and make them inline

//all controller functions
inline int controller::CgetScore() {
    return m->getScore();
}

inline void controller::CsetDir(Movement variable) {
    m->setDirection(variable);
}

inline Movement controller::CgetDir() {
    return m->getDir();
}

inline void controller::CpressEvent(QKeyEvent *e) {
    v->keyPressEvent(e);
}

inline void controller::CdoDrawing() {
    v->doDrawing();
}

inline void controller::CFoodCollision() {
    m->foodCollision();
}

inline void controller::Cmove() {
    m->move();
}

inline void controller::CcheckCollision() {
    m->checkCollision();
}

inline void controller::CobstacleCollison() {
    m->obstacleCollision();
}

inline void controller::CKillTimer() {
    v->killTimer(CgetTimer());
}

inline int controller::CgetTimer() {
    return v->getTimerId();
}

inline coordinate& controller::CgetApple() {
   return m->getApple();
}

inline coordinate& controller::CgetSnake() {
    return m->getSnake();
}

inline std::vector<coordinate>& controller::CgetPlaced() {
    return m->getPlaced();
}

inline std::vector<coordinate>& controller::CgetSnakeVec() {
    return m->getSnakeVec();
}

inline void controller::CinitGame() {
    m->initGame();
}

inline bool controller::CgamePlay() {
    return m->GetGamePlay();
}

//end controller functions

/*--------------------------------------------------------------------*/

//all view functions
//constructor
view::view(controller* c) : c(c) {
    std::cout << "view constructor2" << std::endl;
    setStyleSheet("background-color:grey;");

    //set the paly area size
    resize(B_WIDTH, B_HEIGHT);
    //load the png files for the game
    loadImages();
    timerId = startTimer(DELAY);
    //init game
    c->CinitGame();

}

inline int view::getTimerId() {
    return timerId;
}

inline void view::loadImages() {

    std::cout << "load images" << std::endl;
    dot_png.load(":/new/prefix1/dot.png");
    head_png.load(":/new/prefix1/head.png");
    apple_png.load(":/new/prefix1/apple.png");
    obstacle_png.load(":/new/prefix1/obstacle.png");
}


inline void view::initGame() {
    c->CinitGame();
}

inline void view::paintEvent(QPaintEvent *e) {

    Q_UNUSED(e);

    std::cout << "paintevent" << std::endl;

    c->CdoDrawing();
}

inline void view::timerEvent(QTimerEvent *e){
    Q_UNUSED(e);

    if (c->CgamePlay()) {

        c->CFoodCollision();
        c->CobstacleCollison();
        c->CcheckCollision();
        c->Cmove();
    }

    repaint();
}


inline void view::doDrawing() {

    QPainter qp(this);

    std::vector<coordinate> placed_obstacles = c->CgetPlaced();
    std::vector<coordinate> the_snake = c->CgetSnakeVec();

    coordinate apple = c->CgetApple();


    if(c->CgamePlay()) {
         //draw the obstacles
         for(unsigned long int i{}; i < placed_obstacles.size() ; i++) {
            qp.drawImage(placed_obstacles[i].m_x, placed_obstacles[i].m_y, obstacle_png);
         }
         //draw the apple
         qp.drawImage(apple.m_x, apple.m_y, apple_png);
         //draw the snake
         for (unsigned long int i = 0; i < the_snake.size(); i++) {
            if (i == 0) {
                qp.drawImage(the_snake[i].m_x, the_snake[i].m_y, head_png);
            }
            else {
                qp.drawImage(the_snake[i].m_x, the_snake[i].m_y, dot_png);
            }
        }

    }
    else {
        gameOver(qp);
    }
}

inline void view::gameOver(QPainter &qp) {

    QString GameOver = "Game over";
    QString YourScore = "Your score ";
    QString display_score = QString::number(c->CgetScore());
    QSound::play(":/new/prefix1/fail-trombone-03.wav");
    QFont font("Courier", 15, QFont::DemiBold);
    QFontMetrics fm(font);
    int textWidth = fm.width(GameOver);

    qp.setFont(font);
    int h = height();
    int w = width();

    qp.translate(QPoint(w/2, h/2));
    qp.drawText(-textWidth/2, 0, GameOver);
    qp.drawText(-textWidth/2, 20, YourScore+=display_score);
}

inline void view::keyPressEvent(QKeyEvent *e) {

    int key = e->key();

    if ((key == Qt::Key_Left) && (!(c->CgetDir() == Movement::west))) {
        //direction = Movement::east;
        c->CsetDir(Movement::east);
    }

    if ((key == Qt::Key_Right) && (!(c->CgetDir() == Movement::east))) {
        //direction = Movement::west;
        c->CsetDir(Movement::west);
    }

    if ((key == Qt::Key_Up) && (!(c->CgetDir() == Movement::south))) {
        //direction = Movement::north;
        c->CsetDir(Movement::north);
    }

    if ((key == Qt::Key_Down) && (!(c->CgetDir() == Movement::north))) {
        //direction = Movement::south;
        c->CsetDir(Movement::south);
    }

    QWidget::keyPressEvent(e);

}

/*--------------------------------------------------------------------*/

//all model functions are below

inline int model::getScore() {
    return score;
}

inline Movement model::getDir() {
    return direction;
}

inline coordinate& model::getApple() {
    return apple;
}

inline coordinate& model::getSnake() {
    return snake;
}

inline std::vector<coordinate>& model::getPlaced() {
    return placed_obstacles;
}

inline std::vector<coordinate>& model::getSnakeVec() {
    return the_snake;
}

inline void model::initGame() {

    for(int i = 0; i < snake_size; i++) {
            snake.m_x = 150 - (i * BODY_SIZE);
            snake.m_y = 150;
            the_snake.push_back(snake);
        }

    placeFood();
    placeObstacle();
}

inline bool model::GetGamePlay() {
    return GamePlay;
}

inline void model::grow(int amount) {
       coordinate last = the_snake.back();
       for(int i{}; i < amount; i++) {
           the_snake.push_back(last);
       }
   }

inline void model::foodCollision() {

    if((the_snake[0].m_x > apple.m_x - 15 && the_snake[0].m_x < apple.m_x + 15)
            && (the_snake[0].m_y > apple.m_y - 15 && the_snake[0].m_y < apple.m_y + 15)) {
        grow(3);
        score++;
        QSound::play(":/new/prefix1/button-1.wav");
        placeFood();
        if(the_snake.size() % 2) placeObstacle();

    }
}

inline void model::obstacleCollision() {

    for(unsigned long int i{}; i < placed_obstacles.size() ; i++) {
        if ((the_snake[0].m_x > placed_obstacles[i].m_x - 10 && the_snake[0].m_x < placed_obstacles[i].m_x + 10)
            && (the_snake[0].m_y > placed_obstacles[i].m_y - 10 && the_snake[0].m_y < placed_obstacles[i].m_y + 10)) {
            //QSound::play(":/new/prefix1/button-1.wav");
            GamePlay = false;
            break;
        }
            if(!GamePlay) {
                c->CKillTimer();
            }
        }
}



inline void model::move() {

    std::cout << "move()" << std::endl;

    for (int i = the_snake.size()-1; i > 0; i--) {
        the_snake[i] = the_snake[i - 1];
    }

    switch(direction){
            case Movement::north:
                the_snake[0].m_y-=BODY_SIZE;
                break;
            case Movement::south:
                the_snake[0].m_y+=BODY_SIZE;
                break;
            case Movement::east:
                the_snake[0].m_x-=BODY_SIZE;
                break;
            case Movement::west:
                the_snake[0].m_x+=BODY_SIZE;
            break;

            }
}

inline void model::checkCollision() {

    //below for self collision
    for(unsigned long int i{1}; i < the_snake.size() - 1; i++) {
                if(the_snake[0] == the_snake[i]) { GamePlay = false; break; }
            }

    if(!GamePlay) {
        c->CKillTimer();
    }


    //add GamePlay = false to each if() to disable wraparound
    //and enable wall crash
    if(the_snake[0].m_y >= B_HEIGHT){
        the_snake[0].m_y = 0;
    }

    if(the_snake[0].m_y < 0){
        the_snake[0].m_y = B_HEIGHT;
    }

    if(the_snake[0].m_x >= B_WIDTH){
        the_snake[0].m_x = 0;
    }

    if(the_snake[0].m_x < 0){
        the_snake[0].m_x = B_WIDTH;
    }

}

inline void model::placeFood() {

    std::cout << "place food()" << std::endl;

    bool notundersnake{true};
    std::random_device dev;
    std::uniform_int_distribution<int> dist(0,B_HEIGHT-BODY_SIZE);
    coordinate temp((dist(dev)), (dist(dev)));
    //check to make sure apple isnt placed under snake
    for(unsigned long int i{}; i < the_snake.size(); i++ ) {
        if((temp.m_x > the_snake[i].m_x - 15 && temp.m_x < the_snake[i].m_x + 15)
            && (temp.m_y > the_snake[i].m_y - 15 && temp.m_y < the_snake[i].m_y +15))
        { notundersnake = false; break; }
    }
    if(notundersnake) apple = temp;
    std::cout << apple.m_x << " " << apple.m_y << std::endl;
}


inline void model::placeObstacle() {

    bool not_duplicate{true};
    std::random_device dev;
    std::uniform_int_distribution<int> dist(0,B_HEIGHT-BODY_SIZE);
    coordinate p((dist(dev)), (dist(dev)));
    std::cout << p.m_x << " " << p.m_y << std::endl;
    if(placed_obstacles.empty()) placed_obstacles.push_back(p);
    else {
        for(unsigned long int i{}; i < placed_obstacles.size() -1; i++) {
            if(p == placed_obstacles[i]) { not_duplicate = false; break; }
            if((placed_obstacles[i].m_x > apple.m_x - 100 && placed_obstacles[i].m_x < apple.m_x + 100)
                && (placed_obstacles[i].m_y > apple.m_y - 100 && placed_obstacles[i].m_y < apple.m_y + 100)) {
                not_duplicate = false; break;
            }
        }
        if(not_duplicate) { placed_obstacles.push_back(p); }
    }
}

#endif // GAME_H
