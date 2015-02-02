//Marcado

#include "CCamera.h"

#include "debug.h"

CCamera CCamera::CameraControl;

CCamera::CCamera() {
		X = 0;
		Y = 0;

    TargetX = TargetY = NULL;

    TargetMode = TARGET_MODE_NORMAL;
}

void CCamera::OnMove(int MoveX, int MoveY) {
    X += MoveX;
    Y += MoveY;
}

int CCamera::GetX() {
    if(TargetX != NULL) {
        if(TargetMode == TARGET_MODE_CENTER) {
            return *TargetX - (800/2);
        }

        return *TargetX;
    }

    return X;
}

int CCamera::GetY() {
    if(TargetY != NULL) {
        if(TargetMode == TARGET_MODE_CENTER) {
            return *TargetY - (600 / 2);
        }

        return *TargetY;
    }

    return Y;
}

void CCamera::SetPos(int X, int Y) {
    this->X = X;
    this->Y = Y;
}

void CCamera::SetTarget(float* X, float* Y) {
    TargetX = X;
    TargetY = Y;
}
 