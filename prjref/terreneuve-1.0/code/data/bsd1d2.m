function [d1, d2] = BSD1D2(So, T, K, r, sigma);

% [d1, d2] = BSD1D2(So, T, K, r, sigma);
%
% Return the Black-Scholes parameters d1 and d2
% (boundaries of integration for pdf)
%
% So can be a vector or sigma (not both), 
% all other args should be scalar

sigx = sigma.*sqrt(T);
d1 = (-log(K./So) + (r + 1/2 * sigma.^2) * T)./sigx;
d2 = d1 - sigx;