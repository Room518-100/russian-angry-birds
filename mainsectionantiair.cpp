#include <iostream>
// ����������
#include <math.h>

// ������ Linux/Windows
// ��� ������ �������� �������
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
// ��������� ����������� ����
#define ROWS 24
#define COLUMNS 80

// �������� ��������� � ���������
// RangeY = ROWS
// RangeX = COLUMNS
#define RangeY 10000
#define RangeX 1000
// ����� ��� ����������� � ���� �������
char g_acScreen[ROWS][COLUMNS];
// ������������� ������
void InitScreen()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            if (j == COLUMNS - 1)
            {
                // ����� ������
                g_acScreen[i][j] = '\0';
            }
            else
            if (i == 0)
            {
                // ������
                g_acScreen[i][j] = ' ';
            }
            else
            if (j == 0)
            {
                // ��� Y
                g_acScreen[i][j] = '|';
            }
            else
            if (i == ROWS - 1)
            {
                // ��� X
                g_acScreen[i][j] = '_';
            }
            else
            {
                // ��������� ������������
                g_acScreen[i][j] = ' ';
            }
        }
    }
}

// ���������� �����
void ShowScreen()
{
    for (int i = 0; i < ROWS; i++)
    {
        // ���������� ��������� ������
        cout << g_acScreen[i] << endl;
    }
}
// �������������� ��������� � ������� �� ������
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

// �������� ������ ������������ ��������
class CFlyingObject
{
public:
    // ���������� �������
    float m_numX, m_numY;

// ���������� ���������� �������
    // ����� ��� ����������� ���������� ����
    float m_numPrevX, m_numPrevY;
    // �����������
    CFlyingObject()
    {
    }

    // ���������� ������ �� ������
    void Draw()
    {
        // ������ ��� ��������� ����
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

    // ����������� �������
    virtual char GetObjectView() = 0;

    // ����������� ����������
    virtual char GetObjectTrajectory() = 0;

    // ������� ������ ��������� �� �������
    virtual void HandleTimerUpdate(
            int nLeftTimeFromLastCall) = 0;


protected:
// ������� �������� �� ����
    float m_numVelocityX, m_numVelocityY;

    // ������� ��������� �� ����
    float m_numAccelerationX, m_numAccelerationY;

};

// �������� ����
class CAim : public CFlyingObject
{
public:
    // �����������
    CAim()
    {
        m_numX = RangeX - 999 ;
        m_numY = RangeY / 2;

        m_numPrevX = m_numX;
        m_numPrevY = m_numY;

        m_numVelocityX = +150;
        m_numVelocityY = 0;

    }
// ������ ������ ��������� �� �������
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
// ��������� ����������� �������
    virtual char GetObjectView()
    {
        return 'X';
    }

    // ����������� ����������
    virtual char GetObjectTrajectory()
    {
        return '<';
    }

};

// ���� ������
class CRocket : public CFlyingObject
{
public:

    // �����������
    CRocket()
    {
       m_numX = 500;
        m_numY = 0;

        m_numPrevX = m_numX;
        m_numPrevY = m_numY;

        m_numVelocityX = 0;
        m_numVelocityY = 0;

        // ������� ���������
        m_numAccelerationX = 0;
        m_numAccelerationY = 0;

        // ����� �������
        // �� ������ ��������� �����
        // ������ ��� ������� ����� ��� � 5 ���
        // ������ ������ ���� ��������
        m_numFuelMass = 20;

        // ����� ������ ��� �������
        // ������ ������ ���� ��������
        m_numRocketMass = 100;

        // ������ ������� �� ���������
        m_numMassFloatRate = 0;

        // ���� ���������� ����� �� ���������
        m_numJetAngleInDegree = 0;

    }
    // ������ �������
    // ���������� ������� ����� ���� ��������
    float m_numMassFloatRate;

    // ���� ���������� ����� � ��������
    // ���������� ������� ����� ���� ��������
    float m_numJetAngleInDegree;

    // ������ ������ ��������� �� �������
    virtual void HandleTimerUpdate(
            int nLeftTimeFromLastCall)





    {
        // ����������
        m_numX +=
            m_numVelocityX * nLeftTimeFromLastCall /
                1000;
        m_numY +=
            m_numVelocityY * nLeftTimeFromLastCall /
                1000;

        // ��������
        m_numVelocityX +=
            m_numAccelerationX * nLeftTimeFromLastCall /
                1000;
        m_numVelocityY +=
            m_numAccelerationY * nLeftTimeFromLastCall /
                1000;

        //
        // ������� ��� � ���������
        //

        m_numAccelerationX = 0;
        m_numAccelerationY = 0;

        // ��������� ���� �������
        m_numAccelerationY +=
            -9.8; // g = 9.8

       // ��������� ���� �������
        m_numAccelerationY +=
            -9.8; // g = 9.8

        // ��������� ���� ������ ������� ����������� � �������� ��������

        // ������ ��������
        float numVelocity =
            sqrt(m_numVelocityX * m_numVelocityX +
                 m_numVelocityY * m_numVelocityY);

        // ���� ���������� ������� �������� �� ��������� � ��������
        float numAlpha = 90;

        // ������ ����� ������
        float numFullMass =
            m_numFuelMass + m_numRocketMass;

        // ��������� ������������� �������
        if (numVelocity > 0)
        {
            numAlpha =
                180 * acos(m_numVelocityX / numVelocity) / 3.14;

            // ��� �������� �������� �� ��� X
            float numCosAlpha =
                m_numVelocityX / numVelocity;

            // ��� �������� �������� �� ��� Y
            float numSinAlpha =
                m_numVelocityY / numVelocity;

            // ���� ������������� ������� X = A * V^2 = 0.01 * V^2
            float numDragForce = 1.01 * numVelocity * numVelocity;

            // ������� ������������� �� ���������
            m_numAccelerationX +=
                -numDragForce * numCosAlpha / numFullMass;
            m_numAccelerationY +=
                -numDragForce * numSinAlpha / numFullMass;
        }

        // ���� ���������� ����� � ��������
        float numBeta = m_numJetAngleInDegree;

        // ���� ���������� ����� �� ����� ���� �������
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

        // ���� ������� ����
        float numGamma = numAlpha - numBeta;

        // �������� ���� �� ��� X
        float numCosGamma =
            cos(3.14 * numGamma / 180);

        // ��� �������� �� ��� Y
        float numSinGamma =
            sin(3.14 * numGamma / 180);

        // ������� �����������
        m_numFuelMass -=
            m_numMassFloatRate * nLeftTimeFromLastCall / 1000;

        if (m_numFuelMass <= 0)
        {
            m_numMassFloatRate = 0;
        }

        // ���� ��������� F = u * dm/dt = 3000 * dm/dt
        float numThrustForce =
            10000 * m_numMassFloatRate;

        // ������� ���� �� ���������
        m_numAccelerationX +=
            numThrustForce * numCosGamma / numFullMass;
        m_numAccelerationY +=
            numThrustForce * numSinGamma / numFullMass;



    }


protected:
// ��������� ����������� �������
    virtual char GetObjectView()
    {
        return 'R';
    }

    // ����������� ����������
    virtual char GetObjectTrajectory()
    {
        return '*';
    }

private:


    // ����� �������
    float m_numFuelMass;

    // ����� ������ ��� �������
    float m_numRocketMass;

};
// �������� ��������� ����
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
    // ����� ������ ������� ���������
    int tickStart = GetTickCount();
    int tickPrev = tickStart;
    int tickPrevDraw = tickStart;

    // ��� ����� ��������� ��������� ������ � ���� � �������������
    static const int C_nDrawTimeout = 500;
    static const int C_nProcessTimeout = 18;
    // ����� ������ ���������
    static const int C_nProgramTimeout = 10000;

    ShowScreen();

    // ����
    CAim oAim;

    // ���� ������
    CRocket oRocket;
// ������ ����������� ����
    bool bAimDestroyed = false;

    while (true)
    {
        int tickCurrent = GetTickCount();
// �����
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

// ���������
        if (tickCurrent - tickPrev > C_nProcessTimeout &&
            !bAimDestroyed)

        {
            // ������ ���� ��������� ���������� ������� �����
            // ����� ������
            int tickRocketTime = tickCurrent - tickStart;

            // ���������� ����
            float numAimX = oAim.m_numX;
            float numAimY = oAim.m_numY;

            // ���������� ������
            float numRocketX = oRocket.m_numX;
            float numRocketY = oRocket.m_numY;

            if (tickRocketTime > 0 &&
                tickRocketTime <= 2000)
            {
                // ������ ������� ���������� �������
                // �����������
                oRocket.m_numJetAngleInDegree = 10;
                oRocket.m_numMassFloatRate = 5;
            }
            else
            if (tickRocketTime > 2000 &&
                tickRocketTime <= 10000)
            {
                // ������ ������� ���������� �������
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

        // �����������
        if (tickCurrent - tickPrevDraw > C_nDrawTimeout)
        {
            tickPrevDraw = tickCurrent;
            ShowScreen();
        }
    }
    return 0;
}
