function S = SURVPROB(T, B, tB, s, ts, R);
% f = SURVPROB(T, B, tB, s, ts, R)
%
% Calculate survival probability up to time T.
% See DEFPROB for definition of the inputs.
%
% 2005 aloke mukherjee

% disp(sprintf('survprob(%.2f)', T));

if (T == 0)
  S = 1;
else
  Tidx = find(ts==T);
  Bidx = find(tB==T);
  Tminus = ts(Tidx - 1);
  Sminus = SURVPROB(Tminus, B, tB, s, ts, R);
  qminus = DEFPROB(T, B, tB, s, ts, R);
  S = Sminus * (1 - qminus);
end;
