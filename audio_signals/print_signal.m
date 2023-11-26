clc;
%% R0
fprintf("const audio_signal_r0[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (r0(j*32+i) > 0)
            fprintf("0x%08X, ", r0(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(r0(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");
%% R1
fprintf("\nconst audio_signal_r1[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (r1(j*32+i) > 0)
            fprintf("0x%08X, ", r1(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(r1(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");

%% R2
fprintf("\nconst audio_signal_r2[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (r2(j*32+i) > 0)
            fprintf("0x%08X, ", r2(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(r2(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");

%% R3
fprintf("\nconst audio_signal_r3[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (r3(j*32+i) > 0)
            fprintf("0x%08X, ", r3(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(r3(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");

%% R4
fprintf("const audio_signal_r4[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (r4(j*32+i) > 0)
            fprintf("0x%08X, ", r4(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(r4(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");
%% R5
fprintf("\nconst audio_signal_r5[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (r5(j*32+i) > 0)
            fprintf("0x%08X, ", r5(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(r5(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");

%% R6
fprintf("\nconst audio_signal_r6[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (r6(j*32+i) > 0)
            fprintf("0x%08X, ", r6(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(r6(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");

%% R7
fprintf("\nconst audio_signal_r7[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (r7(j*32+i) > 0)
            fprintf("0x%08X, ", r7(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(r7(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");

%% G0
fprintf("\nconst audio_signal_g0[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (g0(j*32+i) > 0)
            fprintf("0x%08X, ", g0(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(g0(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");

%% G1
fprintf("\nconst audio_signal_g1[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (g1(j*32+i) > 0)
            fprintf("0x%08X, ", g1(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(g1(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");

%% G2
fprintf("\nconst audio_signal_g2[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (g2(j*32+i) > 0)
            fprintf("0x%08X, ", g2(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(g2(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");

%% G3
fprintf("\nconst audio_signal_g3[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (g3(j*32+i) > 0)
            fprintf("0x%08X, ", g3(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(g3(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");

%% G4
fprintf("\nconst audio_signal_g4[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (g4(j*32+i) > 0)
            fprintf("0x%08X, ", g4(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(g4(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");

%% G5
fprintf("\nconst audio_signal_g5[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (g5(j*32+i) > 0)
            fprintf("0x%08X, ", g5(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(g5(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");

%% G6
fprintf("\nconst audio_signal_g6[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (g6(j*32+i) > 0)
            fprintf("0x%08X, ", g6(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(g6(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");

%% G7
fprintf("\nconst audio_signal_g7[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (g7(j*32+i) > 0)
            fprintf("0x%08X, ", g7(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(g7(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");

%% B0
fprintf("\nconst audio_signal_b0[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (b0(j*32+i) > 0)
            fprintf("0x%08X, ", b0(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(b0(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");

%% B1
fprintf("\nconst audio_signal_b1[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (b1(j*32+i) > 0)
            fprintf("0x%08X, ", b1(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(b1(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");

%% B2
fprintf("\nconst audio_signal_b2[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (b2(j*32+i) > 0)
            fprintf("0x%08X, ", b2(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(b2(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");

%% B3
fprintf("\nconst audio_signal_b3[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (b3(j*32+i) > 0)
            fprintf("0x%08X, ", b3(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(b3(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");
%% B4
fprintf("\nconst audio_signal_b4[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (b4(j*32+i) > 0)
            fprintf("0x%08X, ", b4(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(b4(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");

%% B5
fprintf("\nconst audio_signal_b5[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (b5(j*32+i) > 0)
            fprintf("0x%08X, ", b5(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(b5(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");

%% B6
fprintf("\nconst audio_signal_b6[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (b6(j*32+i) > 0)
            fprintf("0x%08X, ", b6(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(b6(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");

%% B7
fprintf("\nconst audio_signal_b7[] = {\n");
for j=0:15
    fprintf("\t");
    for i = 1:32
        if (b7(j*32+i) > 0)
            fprintf("0x%08X, ", b7(j*32+i));
        else 
            fprintf("0x%s, ", ndec2hex(b7(j*32+i), 32));
        end
    end
    fprintf("\n");
end
fprintf("};\n");



