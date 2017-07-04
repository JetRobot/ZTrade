function p = DEFPROB(T, B, tB, s, ts, R);
% p = DEFPROB(T, B, tB, s, ts, R)
%
% Calculate default probability for period T.
% This is defined as the probability of default from time
% T to T + delta conditional on no previous default.
%
% inputs:
% T - start of period to calculate default probability
% B - vector of discount factors
% tB - times corresonding to discount factors in B
% s - vector of spreads vs. riskfree
% ts - vector of times corresponding to spreads in s
%  this set of times is also used for the periods in which
%  default can occur
% R - recovery rate 
%
% T should be an entry in tB and ts
% 
% Can roughly reproduce example in Hull 6th ed s21.3 (except
% we plug in the spread and check if get out the prob, also
% not doing accrual):
%
% tB = 0:.5:5;
% B = exp(-0.05*tB);
% ts = 0:5;
% s = (0.0511/4.0704) * ones(1, length(ts));
% R = 0.4;
% T = 5;
% p = defprob(T, B, tB, s, ts, R);
% p should equal 0.0200 - same for T = 1,2,3,4,5
% 
% another test:
% change spreads to 0.0100 -> get something close to 1.61% as 
% Hull says on p. 513.
%
% 2005 aloke mukherjee

% disp(sprintf('defprob(%.2f)', T));

if (T == 0) 
  p = 0;
else
  Tidx = find(ts==T);
  Bidx = find(tB==T);
  Tminus = ts(Tidx - 1);
  Sminus = SURVPROB(Tminus, B, tB, s, ts, R);
  Fminus = FEES(Tminus, B, tB, s, ts, R);
  DF = B(Bidx);

  % default occurs halfway between spreads
  Tdefault = T - (ts(Tidx) - ts(Tidx - 1))/2;
  Tdefaultidx = find(tB==Tdefault);
  DFd = B(Tdefaultidx);
  % disp(sprintf('Dfd = %.5f', DFd));

  num = Fminus * (s(Tidx)/s(Tidx-1) - 1) + DF * s(Tidx) * Sminus;
  % denom = DF * Sminus * (1 - R + s(Tidx));
  denom = Sminus * (DFd * (1 - R) + DF * s(Tidx));
  p = num/denom;
end;