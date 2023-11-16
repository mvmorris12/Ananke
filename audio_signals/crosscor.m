% [xr,tr] = xcorr(fft_model_r, fft_test, 1024);
% [xg,tg] = xcorr(fft_model_g, fft_test, 1024);
% [xb,tb] = xcorr(fft_model_b, fft_test, 1024);
% model_r_rev(4096)=0;
% for i=1:4095
%     model_r_rev(i) = model_b(4096-i);
% end
% a = abs(fft(model_b));
% b = abs(fft(model_r_rev));
% model_r_rev(2) = 9;

