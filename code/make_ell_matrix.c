#include "defs.h"
#include "fdefs.h"

void
make_ell_matrix()
{
    double phi_rad ;
    double theta_rad ;
    double psi_rad ;
    double s_phi,c_phi ;
    double s_theta,c_theta ;
    double s_psi,c_psi ;

    phi_rad = phi * PI / 180. ;
    theta_rad = theta * PI / 180. ;
    psi_rad = psi * PI / 180. ;
    s_phi = sin(phi_rad) ;
    s_theta = sin(theta_rad) ;
    s_psi = sin(psi_rad) ;
    c_phi = cos(phi_rad) ;
    c_theta = cos(theta_rad) ;
    c_psi = cos(psi_rad) ;
    ell_matrix_inv[0][0] = c_theta*c_phi ;
    ell_matrix_inv[0][1] = c_theta*s_phi ;
    ell_matrix_inv[0][2] = -s_theta ;
    ell_matrix_inv[1][0] = s_psi*s_theta*c_phi-c_psi*s_phi ;
    ell_matrix_inv[1][1] = s_psi*s_theta*s_phi+c_psi*c_phi ;
    ell_matrix_inv[1][2] = c_theta*s_psi ;
    ell_matrix_inv[2][0] = c_psi*s_theta*c_phi+s_psi*s_phi ;
    ell_matrix_inv[2][1] = c_psi*s_theta*s_phi-s_psi*c_phi ;
    ell_matrix_inv[2][2] = c_theta*c_psi ;
    transpose(ell_matrix_inv,ell_matrix) ;
}
