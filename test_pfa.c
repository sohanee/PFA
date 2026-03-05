/******************************************************/
/* Program to run and test the integration functions. */
/* Creation date: 31 July, 2025                       */
/******************************************************/

#include "pfa.h"

/* Code here anything you want to test your pfa functions and to show your numerical results */
int main()
{
  if (!init_integration("gauss2", 0.1))
  {
    fprintf(stderr, "init_integration failed\n");
    return 2;
  }
  printf(" Teste phi et PHI\n\n");
  printf("    => phi(0) = %f (attendu: 0.398942)\n", phi(0));
  printf("    => PHI(0) = %f (attendu: 0.500000)\n", PHI(0));
  printf("    => phi(1) = %f (attendu: 0.241971)\n", phi(1));
  printf("    => PHI(1) = %f (attendu: 0.841345)\n", PHI(1));
  printf("\n");

  printf("\n========================================\n\n");

  printf(" Cas limites :\n\n");

  Option bad1 = {CALL, 100.0, 100.0, 0.0, 0.05, 0.2};
  printf("    => T=0 call = %f (attendu: 0.0)\n", optionPrice(&bad1));

  Option bad2 = {CALL, 100.0, 100.0, -5.0, 0.05, 0.2};
  printf("    => T<0 call = %f (attendu: 0.0)\n", optionPrice(&bad2));

  Option bad3 = {CALL, 100.0, 100.0, 1.0, 0.05, 0.0};
  printf("    => sigma=0 call = %f (attendu: 0.0)\n\n", optionPrice(&bad3));

  printf(" Prix Call en fonction de K (S0=100, T=2, mu=0.05, sig=0.02)\n\n");
  double ks[] = {99, 100, 101, 102, 103};
  double expected_call[] = {11.5171, 10.5172, 9.51768, 8.51912, 7.52332};
  for (int i = 0; i < 5; i++) {
      Option call = {CALL, 100.0, ks[i], 2.0, 0.05, 0.02};
      printf("    => K=%.0f : %f (attendu: %f)\n", ks[i], optionPrice(&call), expected_call[i]);
  }

  printf("\n Prix Call en fonction de T (S0=100, K=101, mu=0.05, sig=0.02)\n\n");
  double ts[] = {1.8, 1.9, 2.0, 2.1, 2.2};
  double expected_call_t[] = {8.41857, 8.9667, 9.51768, 10.0715, 10.6281};
  for (int i = 0; i < 5; i++) {
      Option call = {CALL, 100.0, 101.0, ts[i], 0.05, 0.02};
      printf("    => T=%.1f : %f (attendu: %f)\n", ts[i], optionPrice(&call), expected_call_t[i]);
  }

  printf("\n Prix Put en fonction de K (S0=100, T=2, mu=0.05, sig=0.02)\n\n");
  double ks_put[] = {113, 114, 115, 116, 117};
  double expected_put[] = {2.87264, 3.70203, 4.59834, 5.53979, 6.50911};
  for (int i = 0; i < 5; i++) {
      Option put = {PUT, 100.0, ks_put[i], 2.0, 0.05, 0.02};
      printf("    => K=%.0f : %f (attendu: %f)\n", ks_put[i], optionPrice(&put), expected_put[i]);
  }

  printf("\n Prix Put en fonction de T (S0=100, K=115, mu=0.05, sig=0.02)\n\n");
  double expected_put_t[] = {5.61998, 5.10223, 4.59834, 4.11292, 3.65046};
  for (int i = 0; i < 5; i++) {
      Option put = {PUT, 100.0, 115.0, ts[i], 0.05, 0.02};
      printf("    => T=%.1f : %f (attendu: %f)\n", ts[i], optionPrice(&put), expected_put_t[i]);
  }

  double probs[3] = {0.9, 0.05, 0.05};
  InsuredClient client = {1.0, 1.0, probs};

  printf("\n========================================\n");

  printf("\n ClientPDF_X1X2 (m=1, s=1)\n\n");
  double xs[] = {0, 1, 2, 3};
  double expected_pdf[] = {0, 0.0196294, 0.0726663, 0.101814};
  for (int i = 0; i < 4; i++) {
      printf("    => x=%.0f : %f (attendu: %f)\n", xs[i], clientPDF_X1X2(&client, xs[i]), expected_pdf[i]);
  }

  printf("\n ClientCDF_X1X2 (m=1, s=1)\n\n");
  double expected_cdf[] = {0, 0.00490103, 0.0517347, 0.141439};
  for (int i = 0; i < 4; i++) {
      printf("    => x=%.0f : %f (attendu: %f)\n", xs[i], clientCDF_X1X2(&client, xs[i]), expected_cdf[i]);
  }

  printf("\n ClientCDF_S (m=1, s=1, p0=0.9, p1=p2=0.05)\n\n");
  double expected_s[] = {0.9, 0.908178, 0.921561, 0.934036};
  for (int i = 0; i < 4; i++) {
      printf("    => x=%.0f : %f (attendu: %f)\n", xs[i], clientCDF_S(&client, xs[i]), expected_s[i]);
  }

  printf("\n========================================\n\n");

  return 0;
}
