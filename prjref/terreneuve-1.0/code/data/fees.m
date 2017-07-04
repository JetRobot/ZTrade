function f = FEES(T, B, tB, s, ts, R);
% f = FEES(T, B, tB, s, ts, R)
%
% Calculate fees leg of a CDS.  
% See DEFPROB for definition of the inputs.
%
% 2005 aloke mukherjee

% disp(sprintf('fees(%.2f)', T));

if (T == 0)
  f = 0;
else
  Tidx = find(ts==T);
  Bidx = find(tB==T);
  Tminus = ts(Tidx - 1);
  Fminus = FEES(Tminus, B, tB, s, ts, R);
  Sminus = SURVPROB(Tminus, B, tB, s, ts, R);
  qminus = DEFPROB(T, B, tB, s, ts, R);
  DF = B(Bidx);

  f = Fminus * s(Tidx)/s(Tidx - 1) + DF * s(Tidx) * Sminus * (1 - qminus);
end;
