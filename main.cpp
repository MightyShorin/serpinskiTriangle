#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>

int main() {

    const int maxPoints = 100000; // Максимальное количество точек

    const int windowWidth = 800;
    const int windowHeight = 800;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Serpinski Triangle");
    window.setFramerateLimit(20); // обновление экрана 60 раз в секунду

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
        window.display();
    }

    return 0;
}
