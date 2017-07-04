function [SP, CP, D, C] = BINTREE(M, So, T, K, r, sigma, otype, extype);
% [SP, CP, D, C] = BINTREE(M, So, T, K, r, sigma, otype, extype);
%
% Use binomial tree method to find American claim process given
% M - number of steps
% So - initial stock price
% T - time to maturity
% K - strike
% r - risk-free rate
% sigma - volatility of the stock
% otype - 'call' or 'put'
% extype - 'a' (american) or 'e' (european)
%
% If option is American, claim process value at each node is 
% the max(discounted expectation, claim value).
% e.g. for a call, if S - K at a given node is greater than 
% then the discounted next-step claim values this value will be
% placed in the tree instead.
%
% returns
% SP - stock process, CP - claim process
% D - delta process/replicating stock holding, C - replicating cash holding
% The value of the option is CP(1,1)
% Initial delta is D(1,1)
%
% Constructed to show all tree values - a more efficient implementation
% can be implemented using a single row vector.  See
% http://epubs.siam.org/sam-bin/getfile/SIREV/articles/39326.pdf

% stock process
SP = zeros(M + 1) * NaN; 

% claim process
CP = zeros(M + 1) * NaN;

% delta process
D = zeros(M + 1) * NaN;

% cash process
C = zeros(M + 1) * NaN;

dt = T/M;

% jarrow rudd
mu = (r - 1/2 * sigma^2);
u = exp(mu * dt + sigma * sqrt(dt));
d = exp(mu * dt - sigma * sqrt(dt));

% cox ross rubinstein
% u = exp(sigma * sqrt(dt));
% d = 1/u;

% prob is same either way
q = (exp(r * dt) - d)/(u-d);

disp(sprintf('u = %.4f, d = %.4f, q = %.4f', u, d, q));

% construct stock price process
% SP(1:M + 1,1) = So * ones(M + 1,1);
SP(1,1) = So;

% for each column going forward in time
for i = 2:M + 1
  % for each row
  for n = 1:(i-1)
    SP(n, i) = SP(n, i - 1) * u;
  end;
  % SP(i:M + 1, i) = SP(i-1, i-1) * d * ones(M - i + 2,1);
  SP(i, i) = SP(i-1, i-1) * d;
end;

% find time T claim values (assume call)
CP(:,M + 1) = payoff(otype, SP(:,M + 1), K);
discount = exp(-r * dt);

% construct claim process
% for each column going backwards in time
for i = M:-1:1
  % for each row
  for n = 1:i
    temp = discount * (q * CP(n, i + 1) + (1 - q) * CP(n + 1, i + 1));
    if (extype == 'e')
      CP(n, i) = temp;
    elseif (extype == 'a')
      CP(n, i) = max(temp, payoff(otype, SP(n, i), K));
      if (temp ~= CP(n, i)) 
        disp(sprintf('option exercised at time %d, row %d', i - 1, n - 1));
      end;
    else
      disp(sprintf('unknown exercise type %s', extype));
    end;
  end;
end;

disp(sprintf('%s price is %.5f', otype, CP(1,1)));;

% construct delta process
% for each column going forwards in time
for i = 1:M
  % for each row
  for n = 1:i
    D(n, i) = (CP(n, i+1) - CP(n+1, i+1))/(SP(n, i+1) - SP(n+1, i+1));
    C(n, i) = (CP(n, i) - D(n, i) * SP(n, i));
  end;
end;
disp(sprintf('time 0 delta is %.5f', D(1,1)));
