#include "MassSpringSystemSimulator.h"



MassSpringSystemSimulator::MassSpringSystemSimulator()
{
    m_fMass = 1;
    m_fStiffness = 1;
    m_fDamping = 1;
    m_iIntegrator = 0;
    useGravity = 0;
}

const char* MassSpringSystemSimulator::getTestCasesStr()
{
    
    return "Demo1, Demo2, Demo3, Demo4";
}

void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass* DUC)
{
    this->DUC = DUC;
    TwType TW_TYPE_INTEGRATOR = TwDefineEnumFromString("Integrator", "Euler, Placeholder, Midpoint");
    switch (m_iTestCase) {
    case 0:
    case 1:
    case 2:
        TwRemoveVar(DUC->g_pTweakBar, "Timestep");
        break;
    case 3:
        TwAddVarRW(DUC->g_pTweakBar, "Integrator", TW_TYPE_INTEGRATOR, &m_iIntegrator, "");
        TwAddVarRW(DUC->g_pTweakBar, "Gravity", TW_TYPE_BOOLCPP, &useGravity, "");
        TwAddVarRW(DUC->g_pTweakBar, "ControlPoint", TW_TYPE_INT32, &controlIndex, "min=0 max=99");
        break;
    default:
        break;
    }

}

void MassSpringSystemSimulator::reset()
{
    controlIndex = 0;
    planeObstacles.clear();
    mpList.clear();
    springList.clear();
    useGravity = false;
    sphereObstacles.clear();
}

void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext)
{
    DUC->setUpLighting(Vec3(), 0.4 * Vec3(1, 1, 1), 100, 0.6 * Vec3(0.97, 0.86, 1));
    int i = 0;
    for (auto& point : mpList) {
        if (m_iTestCase == 3 && controlIndex == i) {
            DUC->setUpLighting(Vec3(), 0.4 * Vec3(1, 1, 1), 100, 0.6 * Vec3(0.1, 1, 0.1));
            DUC->drawSphere(point.position, 0.1);
            DUC->setUpLighting(Vec3(), 0.4 * Vec3(1, 1, 1), 100, 0.6 * Vec3(0.97, 0.86, 1));
        }
        else {
            DUC->drawSphere(point.position, 0.1);
        }
        i++;
    }
    DUC->beginLine();
    for (auto& spring : springList) {
        spring.draw(DUC, mpList[spring.masspoint1].position, mpList[spring.masspoint2].position);
    }
    DUC->endLine();

    for (auto& sphere : sphereObstacles) {
        sphere.Draw(DUC);
    }
}

void MassSpringSystemSimulator::notifyCaseChanged(int testCase)
{
    m_iTestCase = testCase;
    switch (testCase) {
    case 0:
        m_iIntegrator = EULER;
        setupdemoscene();
        simulateTimestep(0.005);
        cout << "Euler: Masspoint 1 Position: " << mpList[0].position << " Velocity: " << mpList[0].velocity << "\n";
        cout << "Euler: Masspoint 2 Position: " << mpList[1].position << " Velocity: " << mpList[1].velocity << "\n\n\n";
        m_iIntegrator = MIDPOINT;
        setupdemoscene();
        simulateTimestep(0.005f);
        cout << "Midpoint: Masspoint 1 Position: " << mpList[0].position << " Velocity: " << mpList[0].velocity << "\n";
        cout << "Midpoint: Masspoint 2 Position: " << mpList[1].position << " Velocity: " << mpList[1].velocity << "\n";
        
        break;
    case 1:
        m_iIntegrator = EULER;
        setupdemoscene();
        break;
    case 2:
        m_iIntegrator = MIDPOINT;
        setupdemoscene();
        for (int i = 0; i < 10; ++i) {
            simulateTimestep(0.005f);
        }
        break;
    case 3:
        m_iIntegrator = EULER;
        setupComplexScene();
        break;
    default:
        break;
    }

}

void MassSpringSystemSimulator::setupdemoscene()
{
    reset();
    m_fMass = 10;
    m_fStiffness = 40;
    addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false);
    addMassPoint(Vec3(0, 4, 0), Vec3(1, 0, 0), false);
    addSpring(0, 1, 1);
}

void MassSpringSystemSimulator::setupComplexScene() {
    reset();
    m_fMass = 20;
    m_fStiffness = 100;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            addMassPoint(Vec3(i, j, 0), Vec3(0, 0, 0), false);
        }
    }
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            addSpring(i*10 + j, i*10+j+1, 0.9);
            addSpring(i * 10 + j, (i + 1) * 10 + j, 0.9);
        }
    }

    for (int i = 0; i < 9; ++i) {
        addSpring(90 + i, 90 + i + 1, 0.9); //Hardcoded
        addSpring(10 * i + 9, (i + 1) * 10 + 9, 0.9);
    }

    planeObstacles.push_back(Plane(Plane::XZ, -1));
    //planeObstacles.push_back(Plane(Plane::XY, -4)); //Just as example to build more walls
    //planeObstacles.push_back(Plane(Plane::XY, 4));

    //sphereObstacles.push_back(Sphere(Vec3(1, 2, 0), 1)); //Spawn a Sphere amidst the different Points

}

void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
{
    Point2D mouseDiff;
    mouseDiff.x = m_trackmouse.x - m_oldtrackmouse.x;
    mouseDiff.y = m_trackmouse.y - m_oldtrackmouse.y;
    if (mouseDiff.x != 0 || mouseDiff.y != 0)
    {
        Mat4 worldViewInv = Mat4(DUC->g_camera.GetWorldMatrix() * DUC->g_camera.GetViewMatrix());
        worldViewInv = worldViewInv.inverse();
        Vec3 inputView = Vec3((float)mouseDiff.x, (float)-mouseDiff.y, 0);
        Vec3 inputWorld = worldViewInv.transformVectorNormal(inputView);
        float inputScale = 0.00002f;
        inputWorld = inputWorld * inputScale;
        applyExternalForce(inputWorld);
    }
}

void MassSpringSystemSimulator::doEuler(float timestep) {
    std::vector<Vec3> accel(mpList.size());

    for (auto& spring : springList) {
        MassPoint mp1 = mpList[spring.masspoint1];
        MassPoint mp2 = mpList[spring.masspoint2];

        float force = calculateSpringForce(spring);
       

        Vec3 direction1 = getNormalized(mp1.position - mp2.position) * force;
        Vec3 direction2 = getNormalized(mp2.position - mp1.position) * force;


        accel[spring.masspoint1] += direction1;
        accel[spring.masspoint2] += direction2;
    }

    for (int i = 0; i < mpList.size(); ++i) {
        MassPoint& mp = mpList[i];
        if (mp.isFixed) {
            continue;
        }
        mp.position += mp.velocity * timestep;
        mp.velocity += (accel[i]/mp.mass) * timestep;
        if (useGravity) {
            mp.velocity += GRAVITY * timestep;
        }       
    }
}

void MassSpringSystemSimulator::doMidpoint(float timestep)
{
    std::vector<Vec3> accelOld(mpList.size());
    std::vector<Vec3> midpos(mpList.size());
    std::vector<Vec3> accelMid(mpList.size());

    //Find the Midpoint Position
    for (int i = 0; i < mpList.size(); ++i) {
        Vec3 pos = mpList[i].position + 0.5f * timestep * mpList[i].velocity;
        midpos[i] = pos;
    }

    //Calculate the Forces at the Old Position
    for (auto& spring : springList) {
        MassPoint mp1 = mpList[spring.masspoint1];
        MassPoint mp2 = mpList[spring.masspoint2];

        float force = calculateSpringForce(spring);

        Vec3 direction1 = getNormalized(mp1.position - mp2.position) * force;
        Vec3 direction2 = getNormalized(mp2.position - mp1.position) * force;

        accelOld[spring.masspoint1] += direction1 / mp1.mass;
        accelOld[spring.masspoint2] += direction2 / mp2.mass;
    }

    //Convert values
    for (auto& vel : accelOld) {
        vel *= 0.5f * timestep;
    }

    //Update the Position of the Masspoints
    for (int i = 0; i < mpList.size(); ++i) {
        MassPoint& mp = mpList[i];
        if (mp.isFixed) {
            continue;
        }
        mp.position += timestep * (accelOld[i] + mp.velocity);
    }

    //Calculate the Midpoint acceleration
    for (auto& spring : springList) {
        Vec3 mp1 = midpos[spring.masspoint1];
        Vec3 mp2 = midpos[spring.masspoint2];

        float force = calculateSpringForce(spring);

        Vec3 direction1 = getNormalized(mp1 - mp2) * force;
        Vec3 direction2 = getNormalized(mp2 - mp1) * force;

        accelMid[spring.masspoint1] += direction1;
        accelMid[spring.masspoint2] += direction2;
    }

    //Update the Velocity of all Masspoints
    for (int i = 0; i < mpList.size(); ++i) {
        MassPoint& mp = mpList[i];
        mp.velocity += timestep * (accelMid[i]/mp.mass);
        if (useGravity) {
            mp.velocity += GRAVITY * timestep;
        }
    }
}

float MassSpringSystemSimulator::calculateSpringForce(Spring spring)
{
    MassPoint mp1 = mpList[spring.masspoint1];
    MassPoint mp2 = mpList[spring.masspoint2];

    float currentLength = norm(mp1.position - mp2.position);
    float diff = currentLength - spring.initialLength;


    return -m_fStiffness * diff;
}

 

void MassSpringSystemSimulator::simulateTimestep(float timeStep)
{
    if (m_iTestCase == 0 || m_iTestCase == 1 || m_iTestCase == 2) { //This is just weird
        timeStep = 0.005f;
    }
    switch (m_iIntegrator) {
    case EULER:
        doEuler(timeStep);
        break;
    case MIDPOINT:
        doMidpoint(timeStep);
        break;
    default:
        break;
    }
    for (auto& mp : mpList) {
        for (auto& plane : planeObstacles) {
            plane.resolveCollision(mp);
        }

        for (auto& sphere : sphereObstacles) {
            sphere.resolveCollision(mp);
        }
    }
    
}

void MassSpringSystemSimulator::onClick(int x, int y)
{
    m_trackmouse.x = x;
    m_trackmouse.y = y;
}

void MassSpringSystemSimulator::onMouse(int x, int y)
{
    m_oldtrackmouse.x = x;
    m_oldtrackmouse.y = y;
    m_trackmouse.x = x;
    m_trackmouse.y = y;
}



void MassSpringSystemSimulator::setMass(float mass)
{
    m_fMass = mass;
}

void MassSpringSystemSimulator::setStiffness(float stiffness)
{
    m_fStiffness = stiffness;
}

void MassSpringSystemSimulator::setDampingFactor(float damping)
{
    m_fDamping = damping;
}

int MassSpringSystemSimulator::addMassPoint(Vec3 position, Vec3 Velocity, bool isFixed)
{
    MassPoint nmp(position, Velocity, m_fMass, isFixed);
    
    int index = static_cast<int>(mpList.size());
    mpList.push_back(nmp);
    return index;
}

void MassSpringSystemSimulator::addSpring(int masspoint1, int masspoint2, float initialLength)
{
    Spring nsp(masspoint1, masspoint2, m_fStiffness, initialLength);

    springList.push_back(nsp);
}

int MassSpringSystemSimulator::getNumberOfMassPoints()
{
    return static_cast<int>(mpList.size());
}

int MassSpringSystemSimulator::getNumberOfSprings()
{
    return static_cast<int>(springList.size());
}

Vec3 MassSpringSystemSimulator::getPositionOfMassPoint(int index)
{
    return mpList[index].position;
}

Vec3 MassSpringSystemSimulator::getVelocityOfMassPoint(int index)
{
    return mpList[index].velocity;
}

void MassSpringSystemSimulator::applyExternalForce(Vec3 force)
{
    if (controlIndex < 0 || controlIndex >= mpList.size()) return;
    mpList[controlIndex].position += force;
}
