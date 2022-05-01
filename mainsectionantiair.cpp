#include <iostream>
// Математика
#include <math.h>

// Таймер Linux/Windows
// для оценки текущего времени
#ifdef WIN32

#include <time.h>

#define GetTickCount() clock();

#else

#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

timeval g_tm;

#define GetTickCount() \
    ((gettimeofday(&g_tm, NULL) == 0) ? (g_tm.tv_sec * 1000 + g_tm.tv_usec / 1000) : 0);

#endif

using namespace std;
// Параметры консольного окна
#define ROWS 24
#define COLUMNS 80

// Диапазон координат и скоростей
// RangeY = ROWS
// RangeX = COLUMNS
#define RangeY 10000
#define RangeX 1000
// Буфер для отображения в окне консоли
char g_acScreen[ROWS][COLUMNS];
// Инициализация экрана
void InitScreen()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            if (j == COLUMNS - 1)
            {
                // Конец строки
                g_acScreen[i][j] = '\0';
            }
            else
            if (i == 0)
            {
                // Отступ
                g_acScreen[i][j] = ' ';
            }
            else
            if (j == 0)
            {
                // Ось Y
                g_acScreen[i][j] = '|';
            }
            else
            if (i == ROWS - 1)
            {
                // Ось X
                g_acScreen[i][j] = '_';
            }
            else
            {
                // Свободное пространство
                g_acScreen[i][j] = ' ';
            }
        }
    }
}

// Отобразить экран
void ShowScreen()
{
    for (int i = 0; i < ROWS; i++)
    {
        // Отобразить очередную строку
        cout << g_acScreen[i] << endl;
    }
}
// Преобразование координат в позицию на экране
int ConvertCoordinateXtoScreenPos(
        float numX)
{
    return
         numX * COLUMNS / RangeX + 1;
}

int ConvertCoordinateYtoScreenPos(
        float numY)
{
    return
         ROWS - (numY * ROWS / RangeY) - 2;
}

// Описание класса летательного аппарата
class CFlyingObject
{
public:
    // Координаты объекта
    float m_numX, m_numY;

// Предыдущие координаты объекта
    // Нужны для отображения траектории цели
    float m_numPrevX, m_numPrevY;
    // Конструктор
    CFlyingObject()
    {
    }

    // Изобразить объект на экране
    void Draw()
    {
        // Пишите код отрисовки сюда
        int nPrevX =
            ConvertCoordinateXtoScreenPos(m_numPrevX);
        int nPrevY =
            ConvertCoordinateYtoScreenPos(m_numPrevY);

        if (nPrevY >= 0 &&
            nPrevX >= 0 &&
            nPrevY < ROWS &&
            nPrevX < COLUMNS)
        {
            g_acScreen[nPrevY][nPrevX] =
                GetObjectTrajectory();
        }
        m_numPrevX = m_numX;
        m_numPrevY = m_numY;
int nX =
            ConvertCoordinateXtoScreenPos(m_numX);
        int nY =
            ConvertCoordinateYtoScreenPos(m_numY);

        if (nY >= 0 &&
            nX >= 0 &&
            nY < ROWS &&
            nX < COLUMNS)
        {
            g_acScreen[nY][nX] =
                GetObjectView();
        }
    }

    // Изображение объекта
    virtual char GetObjectView() = 0;

    // Изображение траектории
    virtual char GetObjectTrajectory() = 0;

    // Рассчет нового положения по таймеру
    virtual void HandleTimerUpdate(
            int nLeftTimeFromLastCall) = 0;


protected:
// Текущая скорость по осям
    float m_numVelocityX, m_numVelocityY;

    // Текущее ускорение по осям
    float m_numAccelerationX, m_numAccelerationY;

};

// Описание цели
class CAim : public CFlyingObject
{
public:
    // Конструктор
    CAim()
    {
        m_numX = RangeX - 999 ;
        m_numY = RangeY / 2;

        m_numPrevX = m_numX;
        m_numPrevY = m_numY;

        m_numVelocityX = +150;
        m_numVelocityY = 0;

    }
// Расчет нового положения по таймеру
    virtual void HandleTimerUpdate(
            int nLeftTimeFromLastCall)
    {
        m_numX +=
            m_numVelocityX * nLeftTimeFromLastCall /
                1000;
        m_numY +=
            m_numVelocityY * nLeftTimeFromLastCall /
                1000;
    }

protected:
// Получение изображения объекта
    virtual char GetObjectView()
    {
        return 'X';
    }

    // Изображение траектории
    virtual char GetObjectTrajectory()
    {
        return '<';
    }

};

// Наша ракета
class CRocket : public CFlyingObject
{
public:

    // Конструктор
    CRocket()
    {
       m_numX = 500;
        m_numY = 0;

        m_numPrevX = m_numX;
        m_numPrevY = m_numY;

        m_numVelocityX = 0;
        m_numVelocityY = 0;

        // Текущее ускорение
        m_numAccelerationX = 0;
        m_numAccelerationY = 0;

        // Масса топлива
        // Не должна превышать массу
        // ракеты без топлива более чем в 5 раз
        // Можете менять этот параметр
        m_numFuelMass = 20;

        // Масса ракеты без топлива
        // Можете менять этот параметр
        m_numRocketMass = 100;

        // Расход топлива по умолчанию
        m_numMassFloatRate = 0;

        // Угол отклонения сопла по умолчанию
        m_numJetAngleInDegree = 0;

    }
    // Расход топлива
    // Управляйте ракетой меняя этот параметр
    float m_numMassFloatRate;

    // Угол отклонения сопла в градусах
    // Управляйте ракетой меняя этот параметр
    float m_numJetAngleInDegree;

    // Расчет нового положения по таймеру
    virtual void HandleTimerUpdate(
            int nLeftTimeFromLastCall)





    {
        // Координаты
        m_numX +=
            m_numVelocityX * nLeftTimeFromLastCall /
                1000;
        m_numY +=
            m_numVelocityY * nLeftTimeFromLastCall /
                1000;

        // Скорость
        m_numVelocityX +=
            m_numAccelerationX * nLeftTimeFromLastCall /
                1000;
        m_numVelocityY +=
            m_numAccelerationY * nLeftTimeFromLastCall /
                1000;

        //
        // Рассчет сил и ускорений
        //

        m_numAccelerationX = 0;
        m_numAccelerationY = 0;

        // Добавляем силу тяжести
        m_numAccelerationY +=
            -9.8; // g = 9.8

       // Добавляем силу тяжести
        m_numAccelerationY +=
            -9.8; // g = 9.8

        // Положение носа ракеты считаем совпадающим с вектором скорости

        // Модуль скорости
        float numVelocity =
            sqrt(m_numVelocityX * m_numVelocityX +
                 m_numVelocityY * m_numVelocityY);

        // Угол отклонения вектора скорости от вертикали в градусах
        float numAlpha = 90;

        // Полная масса ракеты
        float numFullMass =
            m_numFuelMass + m_numRocketMass;

        // Учитываем сопротивление воздуха
        if (numVelocity > 0)
        {
            numAlpha =
                180 * acos(m_numVelocityX / numVelocity) / 3.14;

            // Для проекции скорости на ось X
            float numCosAlpha =
                m_numVelocityX / numVelocity;

            // Для проекции скорости на ось Y
            float numSinAlpha =
                m_numVelocityY / numVelocity;

            // Сила сопротивления воздуха X = A * V^2 = 0.01 * V^2
            float numDragForce = 1.01 * numVelocity * numVelocity;

            // Влияние сопротивления на ускорение
            m_numAccelerationX +=
                -numDragForce * numCosAlpha / numFullMass;
            m_numAccelerationY +=
                -numDragForce * numSinAlpha / numFullMass;
        }

        // Угол отклонения сопла в градусах
        float numBeta = m_numJetAngleInDegree;

        // Угол отклонения сопла не может быть большим
        static const float numMaxJetAngle = 30;

        if (numBeta > numMaxJetAngle)
        {
            numBeta = numMaxJetAngle;
        }
        else
        if (numBeta < (-1.) * numMaxJetAngle)
        {
            numBeta = (-1.) * numMaxJetAngle;
        }

        // Угол вектора тяги
        float numGamma = numAlpha - numBeta;

        // Проекция тяги на ось X
        float numCosGamma =
            cos(3.14 * numGamma / 180);

        // Для проекции на ось Y
        float numSinGamma =
            sin(3.14 * numGamma / 180);

        // Топливо уменьшилось
        m_numFuelMass -=
            m_numMassFloatRate * nLeftTimeFromLastCall / 1000;

        if (m_numFuelMass <= 0)
        {
            m_numMassFloatRate = 0;
        }

        // Тяга двигателя F = u * dm/dt = 3000 * dm/dt
        float numThrustForce =
            10000 * m_numMassFloatRate;

        // Влияние тяги на ускорение
        m_numAccelerationX +=
            numThrustForce * numCosGamma / numFullMass;
        m_numAccelerationY +=
            numThrustForce * numSinGamma / numFullMass;



    }


protected:
// Получение изображения объекта
    virtual char GetObjectView()
    {
        return 'R';
    }

    // Изображение траектории
    virtual char GetObjectTrajectory()
    {
        return '*';
    }

private:


    // Масса топлива
    float m_numFuelMass;

    // Масса ракеты без топлива
    float m_numRocketMass;

};
// Проверка поражения цели
bool IsAimDestroyed(
        CAim oAim,
        CRocket oRocket)
{
    int nAimX =
        ConvertCoordinateXtoScreenPos(oAim.m_numX);
    int nAimY =
        ConvertCoordinateYtoScreenPos(oAim.m_numY);

    int nRocketX =
        ConvertCoordinateXtoScreenPos(oRocket.m_numX);
    int nRocketY =
        ConvertCoordinateYtoScreenPos(oRocket.m_numY);

    if (nAimX == nRocketX &&
        nAimY == nRocketY)
    {
        g_acScreen[nAimY][nAimX - 1] = 'y';
        g_acScreen[nAimY][nAimX] = 'p';
        g_acScreen[nAimY][nAimX + 1] = 'a';
        g_acScreen[nAimY - 1][nAimX] = 'y';
        g_acScreen[nAimY + 1][nAimX] = 'a';

        return
            true;
    }


    return false;
}

int main()
{
    InitScreen();
    // Время начала запуска программы
    int tickStart = GetTickCount();
    int tickPrev = tickStart;
    int tickPrevDraw = tickStart;

    // Как часто обновлять положение ракеты и цели в миллисекундах
    static const int C_nDrawTimeout = 500;
    static const int C_nProcessTimeout = 18;
    // Время работы программы
    static const int C_nProgramTimeout = 10000;

    ShowScreen();

    // Цель
    CAim oAim;

    // Наша ракета
    CRocket oRocket;
// Маркер уничтожения цели
    bool bAimDestroyed = false;

    while (true)
    {
        int tickCurrent = GetTickCount();
// Выход
        if (tickCurrent - tickStart > C_nProgramTimeout)
        {
            break;
        }
        if (!bAimDestroyed)
        {
            bAimDestroyed =
                IsAimDestroyed(
                   oAim,
                   oRocket);
        }

// Обработка
        if (tickCurrent - tickPrev > C_nProcessTimeout &&
            !bAimDestroyed)

        {
            // Пишите свои алгоритмы управления ракетой здесь
            // Время полета
            int tickRocketTime = tickCurrent - tickStart;

            // Координаты цели
            float numAimX = oAim.m_numX;
            float numAimY = oAim.m_numY;

            // Координаты ракеты
            float numRocketX = oRocket.m_numX;
            float numRocketY = oRocket.m_numY;

            if (tickRocketTime > 0 &&
                tickRocketTime <= 2000)
            {
                // Первый участок управления ракетой
                // Разгоняемся
                oRocket.m_numJetAngleInDegree = 10;
                oRocket.m_numMassFloatRate = 5;
            }
            else
            if (tickRocketTime > 2000 &&
                tickRocketTime <= 10000)
            {
                // Второй участок управления ракетой
                if (numRocketX < numAimX &&
                    numRocketY < numAimY)
                {
                    oRocket.m_numJetAngleInDegree = -24;
                    oRocket.m_numMassFloatRate = 3;
                }
                else
                if (numRocketX < numAimX &&
                    numRocketY > numAimY)
                {
                    oRocket.m_numJetAngleInDegree = 30;
                    oRocket.m_numMassFloatRate = 4;
                }
            }
            else
            {
                oRocket.m_numJetAngleInDegree = 0;
                oRocket.m_numMassFloatRate = 0;
            }

            oAim.HandleTimerUpdate(
               tickCurrent - tickPrev);
            oRocket.HandleTimerUpdate(
               tickCurrent - tickPrev);
            tickPrev = tickCurrent;
            oAim.Draw();
            oRocket.Draw();

        }

        // Отображение
        if (tickCurrent - tickPrevDraw > C_nDrawTimeout)
        {
            tickPrevDraw = tickCurrent;
            ShowScreen();
        }
    }
    return 0;
}
