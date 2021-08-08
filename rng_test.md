
# RNG QUALITY

The `-r` command line option given to the game binary makes it stream random
bytes to stdout instead of launching the game. This feature allows to test
the quality of the game RNG by piping such output stream into some RNG 
statistics test suite.

The RNG had been tested via the RNG test suite
[dieharder](https://webhome.phy.duke.edu/~rgb/General/dieharder.php)
and here is the resulting report:

```
#=============================================================================#
#            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
#=============================================================================#
   rng_name    |rands/second|   Seed   |
stdin_input_raw|  1.24e+06  |3699772318|
#=============================================================================#
        test_name   |ntup| tsamples |psamples|  p-value |Assessment
#=============================================================================#
   diehard_birthdays|   0|       100|     100|0.74998519|  PASSED  
      diehard_operm5|   0|   1000000|     100|0.63705107|  PASSED  
  diehard_rank_32x32|   0|     40000|     100|0.82806780|  PASSED  
    diehard_rank_6x8|   0|    100000|     100|0.39971336|  PASSED  
   diehard_bitstream|   0|   2097152|     100|0.56311452|  PASSED  
        diehard_opso|   0|   2097152|     100|0.19321179|  PASSED  
        diehard_oqso|   0|   2097152|     100|0.12163519|  PASSED  
         diehard_dna|   0|   2097152|     100|0.19439866|  PASSED  
diehard_count_1s_str|   0|    256000|     100|0.43654413|  PASSED  
diehard_count_1s_byt|   0|    256000|     100|0.55496135|  PASSED  
 diehard_parking_lot|   0|     12000|     100|0.82824986|  PASSED  
    diehard_2dsphere|   2|      8000|     100|0.80731924|  PASSED  
    diehard_3dsphere|   3|      4000|     100|0.56339786|  PASSED  
     diehard_squeeze|   0|    100000|     100|0.88664390|  PASSED  
        diehard_sums|   0|       100|     100|0.01030961|  PASSED  
        diehard_runs|   0|    100000|     100|0.07635527|  PASSED  
        diehard_runs|   0|    100000|     100|0.40859297|  PASSED  
       diehard_craps|   0|    200000|     100|0.45217118|  PASSED  
       diehard_craps|   0|    200000|     100|0.84648805|  PASSED  
 marsaglia_tsang_gcd|   0|  10000000|     100|0.17921050|  PASSED  
 marsaglia_tsang_gcd|   0|  10000000|     100|0.06602577|  PASSED  
         sts_monobit|   1|    100000|     100|0.09408172|  PASSED  
            sts_runs|   2|    100000|     100|0.16817128|  PASSED  
          sts_serial|   1|    100000|     100|0.99026394|  PASSED  
          sts_serial|   2|    100000|     100|0.89640589|  PASSED  
          sts_serial|   3|    100000|     100|0.82088287|  PASSED  
          sts_serial|   3|    100000|     100|0.77933589|  PASSED  
          sts_serial|   4|    100000|     100|0.59627799|  PASSED  
          sts_serial|   4|    100000|     100|0.56336336|  PASSED  
          sts_serial|   5|    100000|     100|0.25751515|  PASSED  
          sts_serial|   5|    100000|     100|0.80744624|  PASSED  
          sts_serial|   6|    100000|     100|0.12581150|  PASSED  
          sts_serial|   6|    100000|     100|0.69392328|  PASSED  
          sts_serial|   7|    100000|     100|0.99826436|   WEAK   
          sts_serial|   7|    100000|     100|0.20553359|  PASSED  
          sts_serial|   8|    100000|     100|0.81075047|  PASSED  
          sts_serial|   8|    100000|     100|0.78804504|  PASSED  
          sts_serial|   9|    100000|     100|0.31666418|  PASSED  
          sts_serial|   9|    100000|     100|0.77860943|  PASSED  
          sts_serial|  10|    100000|     100|0.18675465|  PASSED  
          sts_serial|  10|    100000|     100|0.17625941|  PASSED  
          sts_serial|  11|    100000|     100|0.91446779|  PASSED  
          sts_serial|  11|    100000|     100|0.94839572|  PASSED  
          sts_serial|  12|    100000|     100|0.61453275|  PASSED  
          sts_serial|  12|    100000|     100|0.91294754|  PASSED  
          sts_serial|  13|    100000|     100|0.99818943|   WEAK   
          sts_serial|  13|    100000|     100|0.19661778|  PASSED  
          sts_serial|  14|    100000|     100|0.98938137|  PASSED  
          sts_serial|  14|    100000|     100|0.74928007|  PASSED  
          sts_serial|  15|    100000|     100|0.94398269|  PASSED  
          sts_serial|  15|    100000|     100|0.39114070|  PASSED  
          sts_serial|  16|    100000|     100|0.71135033|  PASSED  
          sts_serial|  16|    100000|     100|0.95974747|  PASSED  
         rgb_bitdist|   1|    100000|     100|0.45184449|  PASSED  
         rgb_bitdist|   2|    100000|     100|0.23954210|  PASSED  
         rgb_bitdist|   3|    100000|     100|0.81419604|  PASSED  
         rgb_bitdist|   4|    100000|     100|0.96637633|  PASSED  
         rgb_bitdist|   5|    100000|     100|0.46309678|  PASSED  
         rgb_bitdist|   6|    100000|     100|0.88137383|  PASSED  
         rgb_bitdist|   7|    100000|     100|0.68336979|  PASSED  
         rgb_bitdist|   8|    100000|     100|0.24351810|  PASSED  
         rgb_bitdist|   9|    100000|     100|0.12107395|  PASSED  
         rgb_bitdist|  10|    100000|     100|0.66421754|  PASSED  
         rgb_bitdist|  11|    100000|     100|0.05102941|  PASSED  
         rgb_bitdist|  12|    100000|     100|0.31056077|  PASSED  
rgb_minimum_distance|   2|     10000|    1000|0.36670104|  PASSED  
rgb_minimum_distance|   3|     10000|    1000|0.05207168|  PASSED  
rgb_minimum_distance|   4|     10000|    1000|0.00902323|  PASSED  
rgb_minimum_distance|   5|     10000|    1000|0.22019355|  PASSED  
    rgb_permutations|   2|    100000|     100|0.81725453|  PASSED  
    rgb_permutations|   3|    100000|     100|0.25568037|  PASSED  
    rgb_permutations|   4|    100000|     100|0.70768871|  PASSED  
    rgb_permutations|   5|    100000|     100|0.40923424|  PASSED  
      rgb_lagged_sum|   0|   1000000|     100|0.14001807|  PASSED  
      rgb_lagged_sum|   1|   1000000|     100|0.41191877|  PASSED  
      rgb_lagged_sum|   2|   1000000|     100|0.30773956|  PASSED  
      rgb_lagged_sum|   3|   1000000|     100|0.82966648|  PASSED  
      rgb_lagged_sum|   4|   1000000|     100|0.78875165|  PASSED  
      rgb_lagged_sum|   5|   1000000|     100|0.66967296|  PASSED  
      rgb_lagged_sum|   6|   1000000|     100|0.94833743|  PASSED  
      rgb_lagged_sum|   7|   1000000|     100|0.77098225|  PASSED  
      rgb_lagged_sum|   8|   1000000|     100|0.88313535|  PASSED  
      rgb_lagged_sum|   9|   1000000|     100|0.72470012|  PASSED  
      rgb_lagged_sum|  10|   1000000|     100|0.96973256|  PASSED  
      rgb_lagged_sum|  11|   1000000|     100|0.45320129|  PASSED  
      rgb_lagged_sum|  12|   1000000|     100|0.96813466|  PASSED  
      rgb_lagged_sum|  13|   1000000|     100|0.19236729|  PASSED  
      rgb_lagged_sum|  14|   1000000|     100|0.48575416|  PASSED  
      rgb_lagged_sum|  15|   1000000|     100|0.98581363|  PASSED  
      rgb_lagged_sum|  16|   1000000|     100|0.41617110|  PASSED  
      rgb_lagged_sum|  17|   1000000|     100|0.23092653|  PASSED  
      rgb_lagged_sum|  18|   1000000|     100|0.98950523|  PASSED  
      rgb_lagged_sum|  19|   1000000|     100|0.59246969|  PASSED  
      rgb_lagged_sum|  20|   1000000|     100|0.25379698|  PASSED  
      rgb_lagged_sum|  21|   1000000|     100|0.75567557|  PASSED  
      rgb_lagged_sum|  22|   1000000|     100|0.65968167|  PASSED  
      rgb_lagged_sum|  23|   1000000|     100|0.20638568|  PASSED  
      rgb_lagged_sum|  24|   1000000|     100|0.99717394|   WEAK   
      rgb_lagged_sum|  25|   1000000|     100|0.67110577|  PASSED  
      rgb_lagged_sum|  26|   1000000|     100|0.87873793|  PASSED  
      rgb_lagged_sum|  27|   1000000|     100|0.00683629|  PASSED  
      rgb_lagged_sum|  28|   1000000|     100|0.94263020|  PASSED  
      rgb_lagged_sum|  29|   1000000|     100|0.39561643|  PASSED  
      rgb_lagged_sum|  30|   1000000|     100|0.37848532|  PASSED  
      rgb_lagged_sum|  31|   1000000|     100|0.53130883|  PASSED  
      rgb_lagged_sum|  32|   1000000|     100|0.42097145|  PASSED  
     rgb_kstest_test|   0|     10000|    1000|0.07999434|  PASSED  
     dab_bytedistrib|   0|  51200000|       1|0.99918332|   WEAK   
             dab_dct| 256|     50000|       1|0.66536735|  PASSED  
Preparing to run test 207.  ntuple = 0
        dab_filltree|  32|  15000000|       1|0.30767199|  PASSED  
        dab_filltree|  32|  15000000|       1|0.96884355|  PASSED  
Preparing to run test 208.  ntuple = 0
       dab_filltree2|   0|   5000000|       1|0.01839137|  PASSED  
       dab_filltree2|   1|   5000000|       1|0.83599796|  PASSED  
Preparing to run test 209.  ntuple = 0
        dab_monobit2|  12|  65000000|       1|0.94119719|  PASSED  
```

On 114 tests, 110 PASSED, 4 are WEAK, and 0 FAILED.
Seems petty much OK ^^.
