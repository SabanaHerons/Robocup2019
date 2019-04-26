aux2 = 0.2;

%Nao1
for i = 1:100
    plot((i/100)*aux2*sin(t) + Y(1),(i/100)*aux2*cos(t) + X(1),'w','LineWidth',0.1)
    plot(aux2*[-1.2 -1.2] + Y(1) + i/1000,aux2*[0.8 -0.8] + X(1),'w','LineWidth',0.5)
    plot(aux2*[1.2 1.2] + Y(1) - i/1000,aux2*[0.8 -0.8] + X(1),'w','LineWidth',0.5)
     
end
plot(aux2*sin(t) + Y(1),aux2*cos(t) + X(1),'k','LineWidth',0.5)

plot(aux2*[-0.6 -1.2] + Y(1),aux2*[0.8 0.8] + X(1),'k','LineWidth',0.5)
plot(aux2*[-1.2 -1.2] + Y(1),aux2*[0.8 -0.8] + X(1),'k','LineWidth',0.5)
plot(aux2*[-1.2 -0.6] + Y(1),aux2*[-0.8 -0.8] + X(1),'k','LineWidth',0.5)

plot(aux2*[0.6 1.2] + Y(1),aux2*[0.8 0.8] + X(1),'k','LineWidth',0.5)
plot(aux2*[1.2 1.2] + Y(1),aux2*[0.8 -0.8] + X(1),'k','LineWidth',0.5)
plot(aux2*[1.2 0.6] + Y(1),aux2*[-0.8 -0.8] + X(1),'k','LineWidth',0.5)

%Nao2
for i = 1:100
    plot((i/100)*aux2*sin(t) + Y(2),(i/100)*aux2*cos(t) + X(2),'w','LineWidth',0.1)
    plot(aux2*[-1.2 -1.2] + Y(2) + i/1000,aux2*[0.8 -0.8] + X(2),'w','LineWidth',0.5)
    plot(aux2*[1.2 1.2] + Y(2) - i/1000,aux2*[0.8 -0.8] + X(2),'w','LineWidth',0.5)
     
end
plot(aux2*sin(t) + Y(2),aux2*cos(t) + X(2),'k','LineWidth',0.5)
 

plot(aux2*[-0.6 -1.2] + Y(2),aux2*[0.8 0.8] + X(2),'k','LineWidth',0.5)
plot(aux2*[-1.2 -1.2] + Y(2),aux2*[0.8 -0.8] + X(2),'k','LineWidth',0.5)
plot(aux2*[-1.2 -0.6] + Y(2),aux2*[-0.8 -0.8] + X(2),'k','LineWidth',0.5)

plot(aux2*[0.6 1.2] + Y(2),aux2*[0.8 0.8] + X(2),'k','LineWidth',0.5)
plot(aux2*[1.2 1.2] + Y(2),aux2*[0.8 -0.8] + X(2),'k','LineWidth',0.5)
plot(aux2*[1.2 0.6] + Y(2),aux2*[-0.8 -0.8] + X(2),'k','LineWidth',0.5)

%Nao3
for i = 1:100
    plot((i/100)*aux2*sin(t) + Y(3),(i/100)*aux2*cos(t) + X(3),'w','LineWidth',0.1)
    plot(aux2*[-1.2 -1.2] + Y(3) + i/1000,aux2*[0.8 -0.8] + X(3),'w','LineWidth',0.5)
    plot(aux2*[1.2 1.2] + Y(3) - i/1000,aux2*[0.8 -0.8] + X(3),'w','LineWidth',0.5)
     
end
plot(aux2*sin(t) + Y(3),aux2*cos(t) + X(3),'k','LineWidth',0.5)
 

plot(aux2*[-0.6 -1.2] + Y(3),aux2*[0.8 0.8] + X(3),'k','LineWidth',0.5)
plot(aux2*[-1.2 -1.2] + Y(3),aux2*[0.8 -0.8] + X(3),'k','LineWidth',0.5)
plot(aux2*[-1.2 -0.6] + Y(3),aux2*[-0.8 -0.8] + X(3),'k','LineWidth',0.5)

plot(aux2*[0.6 1.2] + Y(3),aux2*[0.8 0.8] + X(3),'k','LineWidth',0.5)
plot(aux2*[1.2 1.2] + Y(3),aux2*[0.8 -0.8] + X(3),'k','LineWidth',0.5)
plot(aux2*[1.2 0.6] + Y(3),aux2*[-0.8 -0.8] + X(3),'k','LineWidth',0.5)

%Nao 4
for i = 1:100
    plot((i/100)*aux2*sin(t) + Y(4),(i/100)*aux2*cos(t) + X(4),'w','LineWidth',0.1)
    plot(aux2*[-1.2 -1.2] + Y(4) + i/1000,aux2*[0.8 -0.8] + X(4),'w','LineWidth',0.5)
    plot(aux2*[1.2 1.2] + Y(4) - i/1000,aux2*[0.8 -0.8] + X(4),'w','LineWidth',0.5)
     
end
plot(aux2*sin(t) + Y(4),aux2*cos(t) + X(4),'k','LineWidth',0.5)
 

plot(aux2*[-0.6 -1.2] + Y(4),aux2*[0.8 0.8] + X(4),'k','LineWidth',0.5)
plot(aux2*[-1.2 -1.2] + Y(4),aux2*[0.8 -0.8] + X(4),'k','LineWidth',0.5)
plot(aux2*[-1.2 -0.6] + Y(4),aux2*[-0.8 -0.8] + X(4),'k','LineWidth',0.5)

plot(aux2*[0.6 1.2] + Y(4),aux2*[0.8 0.8] + X(4),'k','LineWidth',0.5)
plot(aux2*[1.2 1.2] + Y(4),aux2*[0.8 -0.8] + X(4),'k','LineWidth',0.5)
plot(aux2*[1.2 0.6] + Y(4),aux2*[-0.8 -0.8] + X(4),'k','LineWidth',0.5)