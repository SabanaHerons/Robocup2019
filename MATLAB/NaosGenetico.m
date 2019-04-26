%% Sistema dinámico con los NAOs
clear all
clc
close all

options = optimoptions('ga','MaxGenerations',1000,'FunctionTolerance', 1e-15);

dt = 0.1;

Balon = [6*rand(1) 9*rand(1)]';
% Balon = [3 5]';

%Esta es una función auxiliar para penalizar la distancia a las esquinas
g = @(x,y,z,w) (x(1) - 3).^2 + (x(2) - 4.5).^2 + ...
                (y(1) - 3).^2 + (y(2) - 4.5).^2 + ...
                (z(1) - 3).^2 + (z(2) - 4.5).^2 + ...
                (w(1) - 3).^2 + (w(2) - 4.5).^2;
            
%Esta función optimiza la posición de NAO con respecto al resto y al balón
% Está funcionando bien con0.3 y 0.2
f = @(x,y,z,w,b) -polyarea([x(1),y(1),z(1),w(1)], [x(2),y(2),z(2),w(2)])...
                + 0.3*(((x(1) - b(1)).^2 + (x(2) - b(2)).^2) + ...
                    ((y(1) - b(1)).^2 + (y(2) - b(2)).^2) + ...	
                    ((z(1) - b(1)).^2 + (z(2) - b(2)).^2) + ...
                    ((w(1) - b(1)).^2 + (w(2) - b(2)).^2)) + 0.2*g(x,y,z,w);
                

NAO1 = [6*rand(1) 9*rand(1)]';
NAO2 = [6*rand(1) 9*rand(1)]';
NAO3 = [6*rand(1) 9*rand(1)]';
NAO4 = [6*rand(1) 9*rand(1)]';
estados = zeros(8,1);
estados(1:2) = NAO1;
estados(3:4) = NAO2;
estados(5:6) = NAO3;
estados(7:8) = NAO4;
X = [estados(1) estados(3) estados(5) estados(7)];
Y = [estados(2) estados(4) estados(6) estados(8)];
while(ispolycw(X, Y))
    NAO1 = [6*rand(1) 9*rand(1)]';
    NAO2 = [6*rand(1) 9*rand(1)]';
    NAO3 = [6*rand(1) 9*rand(1)]';
    NAO4 = [6*rand(1) 9*rand(1)]';
    estados = zeros(8,1);
    estados(1:2) = NAO1;
    estados(3:4) = NAO2;
    estados(5:6) = NAO3;
    estados(7:8) = NAO4;
    X = [estados(1) estados(3) estados(5) estados(7)];
    Y = [estados(2) estados(4) estados(6) estados(8)];
end

m1 = (NAO2(2)-NAO1(2))/abs(NAO2(1)-NAO1(1))
m2 = (NAO3(2)-NAO1(2))/abs(NAO3(1)-NAO1(1))
m3 = (NAO4(2)-NAO1(2))/abs(NAO4(1)-NAO1(1))

refs = zeros(8,1);

for i = 1:70
   i
%    refs(1:2,i) = ga(@(x) f(x,estados(3:4,i),estados(5:6,i),estados(7:8,i),Balon), 2, [], [], [], [], [0 0]', [6 9]',[],[],options);
%    refs(3:4,i) = ga(@(x) f(estados(1:2,i), x,estados(5:6,i),estados(7:8,i),Balon), 2, [], [], [], [], [0 0]', [6 9]',[],[],options);
%    refs(5:6,i) = ga(@(x) f(estados(1:2,i),estados(3:4,i), x,estados(7:8,i),Balon), 2, [], [], [], [], [0 0]', [6 9]',[],[],options);
%    refs(7:8,i) = ga(@(x) f(estados(1:2,i),estados(3:4,i),estados(5:6,i), x,Balon), 2, [], [], [], [], [0 0]', [6 9]',[],[],options);
%    estados(:,i+1) = estados(:,i) + dt*(refs(:,i) - estados(:,i));
   ref1 = ga(@(x) f(x,estados(3:4,i),estados(5:6,i),estados(7:8,i),Balon), 2, [], [], [], [], [0 0]', [6 9]',[],[],options);
   estados(1:2,i+1) = estados(1:2,i) + dt*(ref1' - estados(1:2,i));
   
   ref2 = ga(@(x) f(ref1, x,estados(5:6,i),estados(7:8,i),Balon), 2, [], [], [], [], [0 0]', [6 9]',[],[],options);
   estados(3:4,i+1) = estados(3:4,i) + dt*(ref2' - estados(3:4,i));
   
   ref3 = ga(@(x) f(ref1, ref2, x,estados(7:8,i),Balon), 2, [], [], [], [], [0 0]', [6 9]',[],[],options);
   estados(5:6,i+1) = estados(5:6,i) + dt*(ref3' - estados(5:6,i));
   
   ref4 = ga(@(x) f(ref1, ref2, ref3, x,Balon), 2, [], [], [], [], [0 0]', [6 9]',[],[],options);
   estados(7:8,i+1) = estados(7:8,i) + dt*(ref4' - estados(7:8,i));
end

%%
plot(estados(2,:),estados(1,:),'k--')
hold on
plot(estados(4,:),estados(3,:),'k--')
hold on
plot(estados(6,:),estados(5,:),'k--')
hold on
plot(estados(8,:), estados(7,:),'k--')
X = [estados(1,i+1) estados(3,i+1) estados(5,i+1) estados(7,i+1)];
Y = [estados(2,i+1) estados(4,i+1) estados(6,i+1) estados(8,i+1)];
plot(Y, X,'x','MarkerSize',20, 'MarkerEdgeColor','k');

%Plot de la pelota
t = 0:0.1:2*pi;
plot((1.5*100/500)*sin(t) + Balon(2),((1.5*100/500)*cos(t)+Balon(1)),'k');
for j = 1:70
    aux = j;
    var = 1.5;
    if mod(i,10) == 0
        plot((var*aux/500)*sin(t) + Balon(2),((var*aux/500)*cos(t)+Balon(1)),'k--');
        hold on
    else
        plot((var*aux/500)*sin(t) + Balon(2),((var*aux/500)*cos(t)+Balon(1)),'w--');
        hold on
    end
end
%plot(Balon(2),Balon(1),'o','MarkerSize',15, 'MarkerEdgeColor','k');
axis([-1 10 -1 7])

% Aquí empezamos a plotear la cancha
% Rectangulos
plot([0 0],[0 6],'w')
plot([0 9],[6 6],'w')
plot([9 9],[6 0],'w')
plot([9 0],[0 0],'w')
plot([4.5 4.5],[0 6],'w')

plot([0 0.6],[1.9 1.9],'w')
plot([0.6 0.6],[1.9 4.1],'w')
plot([0.6 0],[4.1 4.1],'w')
plot([0 0],[4.1 1.9],'w')

aux = 9-0.6;
plot([0+aux 0.6+aux],[1.9 1.9],'w')
plot([0.6+aux 0.6+aux],[1.9 4.1],'w')
plot([0.6+aux 0+aux],[4.1 4.1],'w')
plot([0+aux 0+aux],[4.1 1.9],'w')
hold on

% Círculos
t = -0.2:0.01:2*pi;
plot(0.75*cos(t)+4.5,0.75*sin(t)+3,'w')
%plot(0.05*sin(t)+3,0.05*cos(t)+4.5,'k')
plot(0.1*cos(t)+1.3,0.1*sin(t)+3,'w')
plot(0.1*cos(t)+9-1.3,0.1*sin(t)+3,'w')
set(gca,'Color',[0 0.6 0.2]);