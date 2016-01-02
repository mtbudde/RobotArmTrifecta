int pTerm, dTerm;
int Kp = 3;
int Kd = 5;
int integrated_error = 0;
int last_error = 0;
int GUARD_GAIN = 20;
float K = 1;

int updatePid(int targetPosition, int currentPosition)
{
  int error = targetPosition - currentPosition;
  pTerm = Kp * error;
  dTerm = Kd * (error - last_error);
  last_error = error;
  return -constrain(K*(pTerm + dTerm), -127, 127);
}
