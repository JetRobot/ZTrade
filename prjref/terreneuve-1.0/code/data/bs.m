function [y, delta, vega] = BS(So, T, K, r, sigma, otype);
% [y, delta, vega] = BS(So, T, K, r, sigma, otype);
%
% Return the Black-Scholes value of a put/call given
% So - initial stock price
% T - time to maturity
% K - strike
% r - annual risk-free rate
% sigma - volatility of the stock
% otype - 'put' or 'call'
%
% So can be a vector or sigma (not both)
% all other args should be scalar

[d1, d2] = BSD1D2(So, T, K, r, sigma);
vega = So .* sqrt(T/2/pi) .* exp(-d1.^2/2);

if strcmp(otype, 'call') == 1
  delta = normcdf(d1);
  y = So .* delta - K * exp(-r*T) .* normcdf(d2);
elseif strcmp(otype, 'put') == 1
  delta = -1 * normcdf(-d1);
  y = K * exp(-r*T) .* normcdf(-d2) + So .* delta;
else
  y = NaN;
end;
