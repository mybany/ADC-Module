
data = '1111111111111';

value = 0;
for k=1:1:13
       tempdata = fliplr(data);
       value = value + (tempdata(k) - 48) * 1.95^(k - 1);
end

disp(value);