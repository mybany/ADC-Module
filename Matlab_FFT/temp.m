datatemp = data{52};

d = 0;
for i=1:1:13
    d = d + (datatemp(i)-48) * 1.88^(i-1);
end

