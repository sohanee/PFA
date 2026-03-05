/******************************************************/
/* Program to run and test the integration functions. */
/* Creation date: 31 July, 2025                       */
/******************************************************/

#include "pfa.h"

/* Code here anything you want to test your pfa functions and to show your numerical results */
int main()
{
  if (!init_integration("simpson", 0.1))
  {
    fprintf(stderr, "init_integration failed\n");
    return 2;
  }
  printf("Teste phi et PHI\n");
  printf(" =>  phi(0) = %f (attendu: 0.398942)\n", phi(0));
  printf(" =>  PHI(0) = %f (attendu: 0.500000)\n", PHI(0));
  printf(" =>  phi(1) = %f (attendu: 0.241971)\n", phi(1));
  printf(" =>  PHI(1) = %f (attendu: 0.841345)\n", PHI(1));
  printf("\n\n");

  // Test cas limites option
  printf("=== Cas limites ===\n");

  Option bad1 = {CALL, 100.0, 100.0, 0.0, 0.05, 0.2};
  printf("T=0 call = %f (attendu: 0.0)\n", optionPrice(&bad1));

  Option bad2 = {CALL, 100.0, 100.0, -5.0, 0.05, 0.2};
  printf("T<0 call = %f (attendu: 0.0)\n", optionPrice(&bad2));

  Option bad3 = {CALL, 100.0, 100.0, 1.0, 0.05, 0.0};
  printf("sigma=0 call = %f (attendu: 0.0)\n\n", optionPrice(&bad3));

  // Option opt;
  // opt.T = 1;
  // opt.mu = 0;
  // opt.sig = 0.2;

  // opt.type = CALL;
  // opt.S0 = 100;
  // opt.K = 90;
  // double c = optionPrice(&opt);
  // double attendu_c = optionPrice(&opt);
  // printf("  Pour une option de type CALL avec S0=%g, K=%.2g, T=%.2g, mu=%.2g, sig=%.2g:\n", opt.S0, opt.K, opt.T, opt.mu, opt.sig);
  // printf("  prix calculé = %.5g, valeur attendue = %.5g\n", c, attendu_c);

  // opt.type = PUT;
  // opt.S0 = 80;
  // opt.K = 90;
  // double p = optionPrice(&opt);
  // double attendu_p = 12.91;
  // printf("  Pour une option de type PUT  with S0=%.2g, K=%.2g, T=%.2g, mu=%.2g, sig=%.2g:\n", opt.S0, opt.K, opt.T, opt.mu, opt.sig);
  // printf("  prix calculé = %.5g, valeur attendue = %.5g\n", p, attendu_p);

  // InsuredClient client;
  // double p_arr[3] = {0.2, 0.5, 0.3};
  // client.m = 0;
  // client.s = 1;
  // client.p = p_arr;

  // double pdf_x1 = clientPDF_X(&client, 1);
  // double attendu_pdf_x1 = 0.39894;
  // double cdf_x1 = clientCDF_X(&client, 1);
  // double attendu_cdf_x1 = 0.5;
  // printf("  Pour un client avec m=%.2g, s=%.2g, p=[%.2g, %.2g, %.2g]:\n", client.m, client.s, client.p[0], client.p[1], client.p[2]);
  // printf("  clientPDF_X(1): valeur calculée = %.5g, valeur attendue = %.5g\n", pdf_x1, attendu_pdf_x1);
  // printf("  clientCDF_X(1): valeur calculée = %.5g, valeur attendue = %.5g\n", cdf_x1, attendu_cdf_x1);

  // double cdf_S = clientCDF_S(&client, 1);
  // double attendu_cdf_S = 0.486;
  // printf("  clientCDF_S(1): valeur calculée = %.5g, valeur attendue = %.5g\n", cdf_S, attendu_cdf_S);

  return 0;
}
