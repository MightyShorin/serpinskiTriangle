#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <sstream>

int main() {

    const int maxPoints = 100000; // Максимальное количество точек

    const int windowWidth = 800;
    const int windowHeight = 800;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Serpinski Triangle");
    window.setFramerateLimit(20); // обновление экрана 60 раз в секунду

    // Ползунок для FPS
    const float sliderWidth = 200.0f;
    const float sliderHeight = 10.0f;
    sf::RectangleShape sliderBar(sf::Vector2f(sliderWidth, sliderHeight));
    sliderBar.setFillColor(sf::Color::Black);
    sliderBar.setPosition(10, 40);

    sf::RectangleShape sliderHandle(sf::Vector2f(10, 30));
    sliderHandle.setFillColor(sf::Color::Blue);
    sliderHandle.setPosition(10, 30);

    int currentFPS = 30;

    std::vector<sf::Vector2f> trianglePoints;
    sf::Vector2f currentPoint; // начальная точка
    std::vector<sf::CircleShape> points; // вектор всех точек после просчета

    sf::CircleShape pointShape(2.0f); // точка будет кругом радиусом 2
    pointShape.setFillColor(sf::Color::Blue);

    bool drawingStarted = false; // флаг начала рисования
    int pointsDrawn = 0;

    std::srand(static_cast<unsigned>(std::time(nullptr))); // делем сид рандома
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) { // будет продолжаться до тех пор, пока есть события для обработки (очередь кликов)
            if (event.type == sf::Event::Closed) window.close();

            // Обработка кликов мыши
            if(!drawingStarted && event.type == sf::Event::MouseButtonPressed) {
                if ((trianglePoints.size() < 3) && event.mouseButton.button == sf::Mouse::Left) {
                    trianglePoints.emplace_back(event.mouseButton.x, event.mouseButton.y);
                } else if (trianglePoints.size() == 3 && event.mouseButton.button == sf::Mouse::Left) {
                    currentPoint = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                    drawingStarted = true;
                }
            }

            // Обработка перемещения ползунка (только если рисование началось)
            if (drawingStarted && event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                float mouseX = sf::Mouse::getPosition(window).x;

                // Проверяем, если мышь в пределах полосы
                if (mouseX >= sliderBar.getPosition().x &&
                    mouseX <= sliderBar.getPosition().x + sliderWidth - sliderHandle.getSize().x) {

                    sliderHandle.setPosition(mouseX, sliderHandle.getPosition().y);

                    // Обновление FPS
                    float sliderValue = (sliderHandle.getPosition().x - sliderBar.getPosition().x) / (sliderWidth - sliderHandle.getSize().x);
                    currentFPS = static_cast<int>(sliderValue * 2000); // Диапазон FPS: 30 - 2000
                    currentFPS = std::max(30, currentFPS); // Минимум 30 FPS

                    window.setFramerateLimit(currentFPS);

                    }
            }
        }
        window.clear(sf::Color::White);
        if (drawingStarted && pointsDrawn < maxPoints) {
            // Генерация новой точки
            sf::Vector2f target = trianglePoints[std::rand() % 3];
            currentPoint.x = (currentPoint.x + target.x) / 2;
            currentPoint.y = (currentPoint.y + target.y) / 2;

            pointShape.setPosition(currentPoint.x - 1, currentPoint.y - 1); // центрируем точку
            points.push_back(pointShape); // добавляем точку в список

            pointsDrawn++; // увеличиваем счётчик
        }

        // Рисуем все точки
        for (const auto& point : points) {
            window.draw(point);
        }

        window.draw(sliderBar);
        window.draw(sliderHandle);
        window.display();
    }

    return 0;
}
