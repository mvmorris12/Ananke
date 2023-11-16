clc;
fprintf("const audio_signal_r[] = {\n");
for j=0:63
    fprintf("\t");
    for i = 1:64
        if (test_r(j*64+i) > 0)
            fprintf("0x%08X, ", test_r(j*64+i));
        else 
            % fprintf("0x%08X, ", ndec2hex(21908, 8));
            fprintf("0x%s, ", ndec2hex(test_r(j*64+i), 32));
        end
        % count = count+1;
    end
    fprintf("\n");
end
fprintf("};\n\nconst audio_signal_g[] = {\n");
for j=0:63
    fprintf("\t");
    for i = 1:64
        if (test_g(j*64+i) > 0)
            fprintf("0x%08X, ", test_g(j*64+i));
        else 
            % fprintf("0x%08X, ", ndec2hex(21908, 8));
            fprintf("0x%s, ", ndec2hex(test_g(j*64+i), 32));
        end
        % count = count+1;
    end
    fprintf("\n");
end

fprintf("};\n\nconst audio_signal_b[] = {\n");
for j=0:63
    fprintf("\t");
    for i = 1:64
        if (test_b(j*64+i) > 0)
            fprintf("0x%08X, ", test_b(j*64+i));
        else 
            % fprintf("0x%08X, ", ndec2hex(21908, 8));
            fprintf("0x%s, ", ndec2hex(test_b(j*64+i), 32));
        end
        % count = count+1;
    end
    fprintf("\n");
end
fprintf("};\n\n");

% fprintf("%s, ", ndec2hex(-21908, 24));

