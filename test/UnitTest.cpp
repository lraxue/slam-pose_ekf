#include <pose_ekf/KalmanFilter.hpp>

#include <map>
#include <vector>

#define BOOST_TEST_MODULE StateEstimatorTest
#include <boost/test/included/unit_test.hpp>
   
using namespace std;

BOOST_AUTO_TEST_CASE( estimator )
{
/*    using namespace KalmanFilter;
    USING_PART_OF_NAMESPACE_EIGEN
    
    std::vector<std::pair<double,Vector3d> > sc1;
    sc1.push_back(make_pair(1.0,Vector3d(1.0,0.0,0.0)));
    sc1.push_back(make_pair(2.0,Vector3d(0.0,0.0,0.0)));
    sc1.push_back(make_pair(3.0,Vector3d(-1.0,0.0,0.0)));
    sc1.push_back(make_pair(6.0,Vector3d(0.0,0.0,0.0)));

    sc1.push_back(make_pair(7.0,Vector3d(0.0,1.0,0.0)));
    sc1.push_back(make_pair(8.0,Vector3d(0.0,0.0,0.0)));
    sc1.push_back(make_pair(9.0,Vector3d(0.0,-1.0,0.0)));
    sc1.push_back(make_pair(10.0,Vector3d(0.0,0.0,0.0)));

    State x, xp;
    PositionKF estimator;
    estimator.d_t = 0.05;
    estimator.B.corner<3,3>(Eigen::TopLeft) 
	= Matrix3d::Identity() * (estimator.d_t*estimator.d_t/2.0);
    estimator.B.corner<3,3>(Eigen::BottomLeft) 
	= Matrix3d::Identity() * estimator.d_t;
    cout << "B = " << endl << estimator.B << endl;

    estimator.F.setIdentity();
    estimator.F.corner<3,3>(Eigen::TopRight)
	= Matrix3d::Identity() * estimator.d_t;
    cout << "F = " << endl << estimator.F << endl;

    double sigma_a = 1e-1;
    estimator.Q = estimator.B*estimator.B.transpose() * sigma_a*sigma_a;
    cout << "Q = " << endl << estimator.Q << endl;

    estimator.H.setZero();
    estimator.H.corner<3,3>(Eigen::TopLeft)
	= Matrix3d::Identity();
    cout << "H = " << endl << estimator.H << endl;
   
    double sigma_z = 1e-2; 
    estimator.R = Matrix3d::Identity() * sigma_z;
    cout << "R = " << endl << estimator.R << endl;

    estimator.P_k.setIdentity();
    estimator.P_k *= 1.0; // set initial uncertainty

    // evaluate a sample scenario
    std::vector<std::pair<double,Vector3d> >::iterator it = sc1.begin();
    for(double t=0;t<10.0;t+=estimator.d_t)
    {
	// cout << t << " " << x.xi().transpose() << " " << estimator.x_k.xi().transpose() << endl;

	while( it != sc1.end() && (*it).first <= t )
	   it++; 

	Vector3d acc = (*it).second;

	xp.vector() = estimator.F*x.vector() + estimator.B*acc;
	x.vector() = xp.vector();

	estimator.update( acc + 9.81*Vector3d::UnitZ() + sigma_a * Vector3d::Random() );
	// run corrections every second
	if( fabs(round(t) - t) < 1e-3 )
	{
	    estimator.correction( x.xi() + sigma_z * Vector3d::Random() );
	}

    }*/
}

