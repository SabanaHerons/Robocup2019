clear all
clc
%% Prueba Genético%

%X = ga(FITNESSFCN,NVARS,A,b,Aeq,beq,lb,ub)

% x = ga(@(x) x(1)^2 + x(2)^2, 2, [], [], [], [], [0;0],[1, 1]);

%% Iniciamos el Partido
Balon = [6*rand(1) 9*rand(1)]';
% Balon = [3 5]';
NAO1 = [6*rand(1) 10*rand(1)]';
NAO2 = [6*rand(1) 10*rand(1)]';
NAO3 = [6*rand(1) 10*rand(1)]';
NAO4 = [6*rand(1) 10*rand(1)]';

options = optimoptions('ga','MaxGenerations',1000,'FunctionTolerance', 1e-15);

g = @(x,y,z,w) (x(1) - 3).^2 + (x(2) - 4.5).^2 + ...
                (y(1) - 3).^2 + (y(2) - 4.5).^2 + ...
                (z(1) - 3).^2 + (z(2) - 4.5).^2 + ...
                (w(1) - 3).^2 + (w(2) - 4.5).^2;

f = @(x,y,z,w,b) -polyarea([x(1),y(1),z(1),w(1)], [x(2),y(2),z(2),w(2)])...
                + 0.3*(((x(1) - b(1)).^2 + (x(2) - b(2)).^2) + ...
                    ((y(1) - b(1)).^2 + (y(2) - b(2)).^2) + ...
                    ((z(1) - b(1)).^2 + (z(2) - b(2)).^2) + ...
                    ((w(1) - b(1)).^2 + (w(2) - b(2)).^2)) + 0.2*g(x,y,z,w);
                
              

x = ga(@(x) f(x,NAO1,NAO2,NAO3,Balon), 2, [], [], [], [], [0 0]', [6 9]',[],[],options)
Balon
X = [x(1) NAO1(1) NAO2(1) NAO3(1)];
Y = [x(2) NAO1(2) NAO2(2) NAO3(2)];

evolucion = zeros(8,1);
evolucion(1:2) = NAO1;
evolucion(3:4) = NAO2;
evolucion(5:6) = NAO3;
evolucion(7:8) = NAO4;
iter = 10;
for i = 1:iter
    i
    evolucion(1:2,i+1) = ga(@(x) f(x,evolucion(3:4,i),evolucion(5:6,i),evolucion(7:8,i),Balon), 2, [], [], [], [], [0 0]', [6 9]',[],[],options);
    evolucion(3:4,i+1) = ga(@(x) f(evolucion(1:2,i+1), x,evolucion(5:6,i),evolucion(7:8,i),Balon), 2, [], [], [], [], [0 0]', [6 9]',[],[],options);
    evolucion(5:6,i+1) = ga(@(x) f(evolucion(1:2,i+1),evolucion(3:4,i+1), x,evolucion(7:8,i),Balon), 2, [], [], [], [], [0 0]', [6 9]',[],[],options);
    evolucion(7:8,i+1) = ga(@(x) f(evolucion(1:2,i+1),evolucion(3:4,i+1),evolucion(5:6,i+1), x,Balon), 2, [], [], [], [], [0 0]', [6 9]',[],[],options);
    Balon = Balon;
end

evolucion
X = [evolucion(1,iter+1) evolucion(3,iter+1) evolucion(5,iter+1) evolucion(7,iter+1)];
Y = [evolucion(2,iter+1) evolucion(4,iter+1) evolucion(6,iter+1) evolucion(8,iter+1)];

plot(X,Y,'x','MarkerSize',20, 'MarkerEdgeColor','k');
hold on
plot(Balon(1),Balon(2),'o','MarkerSize',20, 'MarkerEdgeColor','b');
axis([-1 7 -1 10])
plot([0 6],[0 0],'k')
plot([6 6],[0 9],'k')
plot([6 0],[9 9],'k')
plot([0 0],[9 0],'k')
set(gca,'Color','g')
figure

plot(evolucion(1,2:end),evolucion(2,2:end))
hold on
plot(evolucion(3,2:end),evolucion(4,2:end))
hold on
plot(evolucion(5,2:end),evolucion(6,2:end))
hold on
plot(evolucion(7,2:end),evolucion(8,2:end))
hold on
plot(Balon(1),Balon(2),'o','MarkerSize',20, 'MarkerEdgeColor','b');
axis([-1 7 -1 10])
plot([0 6],[0 0],'k')
plot([6 6],[0 9],'k')
plot([6 0],[9 9],'k')
plot([0 0],[9 0],'k')
set(gca,'Color','g')