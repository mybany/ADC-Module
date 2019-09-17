%Get the data from TXT

%clear all;

%DATA_ALL = importdata('AD_Result(test).txt');
DATA_ALL = textread('AD_Result(test).txt', '%s');
DATA_ALL = bin2dec(DATA_ALL);

close all;
%DATA = ADResult - 0.5;

DATA = DATA_ALL(0020:1043);

Fs = 60e6;
T = 1/Fs;
L = 1024;
t = (0:1024-1)*T;

Y = fft(DATA);

P2 = abs(Y/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);

f=Fs*(0:(L/2))/L;
plot(f, P1);


y_sp = (abs(P1).^2);

y_sp(1) = 1e-12;
y_sp(513) = 1e-12;

max_ind = max(y_sp);
m = find(y_sp==max(y_sp));
ptot =  sum(y_sp);
p_signal = max(y_sp);
p_noise = ptot-p_signal;
SNDR=10*log10(p_signal/p_noise);
ENOB=(SNDR-1.76)/6.02;

y_sp_temp = y_sp;
y_sp_temp(m) = 1e-12;
pharm=max(y_sp_temp);
SFDR=10*log10(p_signal/pharm);

figure(2);
mag=10*log10(y_sp);


%Calculate SNR  max find 5 order harmonic
%Add 5 extra y_sp to the last!

y_sp_for_SNR = y_sp;
y_sp_flip = flip(y_sp(1:512));
y_sp_for_SNR = [y_sp_for_SNR ; y_sp_flip ; y_sp(2:end) ; y_sp_flip ; y_sp(2:end)];
f_for_SNR = [f f(2:end)+1*Fs/2 f(2:end)+2*Fs/2 f(2:end)+3*Fs/2 f(2:end)+4*Fs/2];

Fin = 50e6 * 251 / 1024; 

harm1 = y_sp_for_SNR((f_for_SNR==Fin));
dBharm1 = 10*log10(harm1);
harm2 = y_sp_for_SNR((f_for_SNR==2*Fin));
dBharm2 = 10*log10(harm2);
harm3 = y_sp_for_SNR((f_for_SNR==3*Fin));
dBharm3 = 10*log10(harm3);
harm4 = y_sp_for_SNR((f_for_SNR==4*Fin));
dBharm4 = 10*log10(harm4);
harm5 = y_sp_for_SNR((f_for_SNR==5*Fin));
dBharm5 = 10*log10(harm5);

harm = harm1+harm2+harm3+harm4+harm5;

SNR = 10*log10(harm/(ptot-harm));






%mag(129) = mag(127);

axes('FontSize',14);
plot(f,mag,'-+')
set(gca, 'LineWidth',1.5)
xlabel('f(Hz)','FontSize',14)
set(gca,'xtick',[0:Fs/16:Fs/2]);

xlabel('f(Hz)','FontSize',14)

ylabel('Power(dB)','FontSize',14 )

%title(stringI,'FontSize',14)
text(2e4,max(mag)-10, sprintf('SFDR = %4.3f',SFDR),'color','r','fontsize',14);
hold on
text(2e4,max(mag)-20, sprintf('SNDR = %4.3f',SNDR),'color','r','fontsize',14);
hold on
text(2e4,max(mag)-30, sprintf('ENOB = %4.3f',ENOB),'color','r','fontsize',14);
hold on
text(2e4,max(mag)-40, sprintf('SNR = %4.3f',SNR),'color','r','fontsize',14);

