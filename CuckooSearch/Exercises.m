% OPTIMISATION PROBLEMS, taken from:
%     1. Jamil, M. & Yang, X. S. A literature survey of benchmark functions
%        for global optimisation problems. Int. J. Math. Model. Numer.
%        Optim. 4, 150 (2013).
%     2. Soneji, H. & Sanghvi, R. C. Towards the improvement of Cuckoo
%        search algorithm. Int. J. Comput. Inf. Syst. Ind. Manag. Appl. 6,
%        77–88 (2014).
%     3. Rakhshani, H. & Rahati, A. Intelligent Multiple Search Strategy
%        Cuckoo Algorithm for Numerical and Engineering Optimization
%        Problems. Arab. J. Sci. Eng. (2016).

% Benchmark functions:
% --
% 0. Sphere from [1]
%[optX,optF] = CuckooSearch(@Sphere,ones(2,1)*[-10 10])

% 1. Beale from [1]
%[optX,optF] = CuckooSearch(@Beale,ones(2,1)*[-35 35])

% 2. Rosenbrock from [1]
[optX,optF] = CuckooSearch(@Rosenbrock,ones(2,1)*[-30 30])

% 3. Bird from [1]
%[optX,optF] = CuckooSearch(@Bird,ones(2,1)*[-2*pi 2*pi])

% 4. Ackley 1 from [1]
%[optX,optF] = CuckooSearch(@Ackley1,ones(2,1)*[-35 35])

% 5. Rastrigin from [2]
%[optX,optF] = CuckooSearch(@Rastrigin,ones(2,1)*[-1 1])

% 6. Weierstrass from [2]
%[optX,optF] = CuckooSearch(@Weierstrass,ones(2,1)*[-.5 .5])

% Benchmark problems:
% --
% 1. Welded Beam Design from [3]
%boundaries  = [ceil(0.125/0.0065),floor(5/0.0065);
%               ceil(0.1/0.0065),floor(10/0.0065);
%               0.1,10; 0.1,10];
%[optX,optF] = CuckooSearch(@WeldedBeamDesign,boundaries);
%optX(1,1:2)   = fix(optX(1,1:2))*0.0065;
%optX,optF,

% 2. Spring Design from [3]
%boundaries  = [0.05 1; 0.25 1.3; 2 15];
%[optX,optF] = CuckooSearch(@SpringDesign,boundaries);
%optX(1,3)   = fix(optX(1,3));
%optX,optF,
