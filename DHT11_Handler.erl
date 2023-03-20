fun(Fields, <<Temp:16/little,Hum:16/little>>) ->
Aux1 = Temp band 16#8000,
Aux2 = Hum band 16#8000,
if
  Temp == 16#8000 -> Temp = 16#0000;
  true -> unknown
end,

if
  Hum == 16#8000 -> Hum = 16#0000;
  true -> unknown
end,

if
  Aux1 /= 0 -> Sign1 = -1;
  true -> Sign1 = 1
end,

if
  Aux2 /= 0 -> Sign2 = -1;
  true -> Sign2 = 1
end,

Exp1 = (Temp bsr 11) band 16#F,
Exp2 = (Hum bsr 11) band 16#F,
Mant1 = (Temp band 16#07FF) / 2048,
Mant2 = (Hum band 16#07FF) / 2048,
Unscaledt = Sign1*Mant1*math:pow(2, Exp1-15),
Unscaledh = Sign2*Mant2*math:pow(2, Exp2-15),
  Fields#{temperatura => Unscaledt*100, humedad => Unscaledh*100, estado => <<"Conectado">>}
end.