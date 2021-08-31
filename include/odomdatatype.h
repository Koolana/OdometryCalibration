#ifndef ODOMDATATYPE_H
#define ODOMDATATYPE_H

enum Tests
{
    LINE,
    SQUARE,
    CIRCLE,
    ERROR,
};

struct OdomDataType {
    float x = 0;
    float y = 0;
    float th = 0;

    float vx = 0;
    float vy = 0;
    float vth = 0;

    bool isClockwise = true;
    int numIter = 0;
};

struct ExperDataType {
    float x = 0;
    float y = 0;
};

struct FullData {
    OdomDataType odom;
    ExperDataType exper;
};

struct CalibrationResults {
    float b = 0;
    float CL = 0;
    float CR = 0;

    Tests typeTest = Tests::ERROR;
};

struct RobotParams {
    float width = 0;
    float linearSpeed = 0;
    float angularSpeed = 0;
};

struct TestData {
    float size = 0;
    int numIter = 1;

    Tests typeTest = Tests::ERROR;
};

#endif // ODOMDATATYPE_H
