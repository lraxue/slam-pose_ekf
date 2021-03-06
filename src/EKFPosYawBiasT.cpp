#include "EKFPosYawBiasT.hpp"
#include <Eigen/LU> 
#include <math.h>
#include "FaultDetection.hpp"

using namespace pose_ekf;


/** CHECK */ 
EKFPosYawBiasT::EKFPosYawBiasT() 
{
    filter =  new ExtendedKalmanFilter::EKF<StatePosYawBias::SIZE>;
    
}

EKFPosYawBiasT::~EKFPosYawBiasT()
{
    delete filter; 
    
}



/** update the filter */ 
void EKFPosYawBiasT::predict( const Eigen::Vector3d &translation_world, const Eigen::Matrix<double, StatePosYawBias::SIZE, StatePosYawBias::SIZE> &Q )
{	
      this->Q = Q; 

    // Rotate from world to world with bias 
    //calculates the rotation from world to world frame corrected by the bias 
    Eigen::Quaterniond R_w2wb;
    R_w2wb=Eigen::AngleAxisd(x.yaw()(0,0), Eigen::Vector3d::UnitZ()); 

    Eigen::Matrix3d R_w2wb_ = Eigen::Matrix3d(R_w2wb);    

    this->Q.block<3,3>(0,0) = R_w2wb_*this->Q.block<3,3>(0,0) *R_w2wb_.transpose();

    //sets the transition matrix 
    Eigen::Matrix<double, StatePosYawBias::SIZE, 1> f;
    f.head<3>() = x.xi() + R_w2wb * translation_world;
    f.tail<1>() = x.yaw();

    //sets the Jacobian of the state transition matrix 
    Eigen::Matrix<double, StatePosYawBias::SIZE, StatePosYawBias::SIZE> J_F
	= jacobianF(translation_world );
  
	
    //updates the Kalman Filter 
    filter->prediction( f, J_F, Q ); 

    
    //get the updated values 
    x.vector()=filter->x; 
    

}	


/** Corrects th KF based on position observation 
Only 2 degree of freedom for rejection, position in x and y */ 
bool EKFPosYawBiasT::correctPosition(const Eigen::Vector3d &position, const Eigen::Matrix3d &covariance, float reject_threshold )
{
  
	Eigen::Matrix<double, 3, StatePosYawBias::SIZE > J_H;
	J_H.setIdentity();
	
	Eigen::Matrix<double, 3, 1 > h
	    = J_H * x.vector(); 

	bool reject = filter->correctionChiSquare< 3, 2 >( position,  covariance, h, J_H, reject_threshold );
	
	x.vector() = filter->x; 
	
	return reject; 
		    
}


/** Corrects th KF based on position and orientation correction observation
Only 2 degree of freedom for rejection, position in x and y */ 
bool EKFPosYawBiasT::correctPositionOrientation(const Eigen::Vector4d &positionOrientation, const Eigen::Matrix4d &covariance, float reject_threshold )
{

    Eigen::Matrix<double, 4,  StatePosYawBias::SIZE> J_H;
    J_H.setIdentity();
    
    Eigen::Matrix<double, 4, 1> h
      = J_H*x.vector(); 
      
    // do the measurement
    bool reject = filter->correctionChiSquare< 4, 2 >( positionOrientation,  covariance, h, J_H, reject_threshold );
    
    x.vector() = filter->x; 
    
    return reject; 


}
	
	
/** Calculates the Jacobian of the transition matrix */ 
Eigen::Matrix<double, StatePosYawBias::SIZE, StatePosYawBias::SIZE> EKFPosYawBiasT::jacobianF( const Eigen::Vector3d &translation_world )
{
    //derivate of the rotation do to yaw bias 
    Eigen::Matrix<double, 3,3> dR_z;
    dR_z << -sin( x.yaw()(0,0) ), -cos( x.yaw()(0,0) ), 0, cos( x.yaw()(0,0) ), -sin( x.yaw()(0,0) ),0 ,0 ,0 ,0; 

    //jacobian 
    Eigen::Matrix<double, StatePosYawBias::SIZE, StatePosYawBias::SIZE> J_F; 
    J_F.setIdentity(); 
    J_F.block<3,1>(0,3)
	= dR_z * translation_world;

    return J_F;
}

void EKFPosYawBiasT::copyState ( const EKFPosYawBiasT& kfd )
{
  
    x = kfd.x; 
    filter->x = kfd.filter->x;
    filter->P = kfd.filter->P;
    
}

void EKFPosYawBiasT::setInitialPosition( const Eigen::Vector3d &position, const Eigen::Matrix3d &covariance )
{
  
    filter->x.head<3>() = position;
    filter->P.block<3,3>(0,0) = covariance; 
    x.vector() = filter->x; 
	    
}

Eigen::Matrix3d EKFPosYawBiasT::getCovariancePosition() 
{
    return filter->P.block<3,3>(0,0);
}

Eigen::Vector3d EKFPosYawBiasT::getPosition()
{
    return x.xi();
}

Eigen::Quaterniond EKFPosYawBiasT::getOrientationCorrection()
{
    Eigen::Quaterniond yawCorrection; 
    yawCorrection = Eigen::AngleAxisd( x.yaw()(0,0), Eigen::Vector3d::UnitZ() ); 
    return yawCorrection;
}

double EKFPosYawBiasT::getOrientationCorrectionCovariance()
{
    return filter->P(3,3);
}

/** configurarion hook */ 
void EKFPosYawBiasT::init(const Eigen::Matrix<double, StatePosYawBias::SIZE, StatePosYawBias::SIZE> &P, const Eigen::Matrix<double,StatePosYawBias::SIZE,1> &x)
{
    Q.setZero(); 
    filter->P = P;
    this->x.vector() = x; 
    filter->x = x; 
}

