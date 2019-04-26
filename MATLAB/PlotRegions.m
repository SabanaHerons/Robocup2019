figure
hold on
% axis off
plot([0 0],[0 6],'w','LineWidth',2)
plot([0 9],[6 6],'w','LineWidth',2)
plot([9 9],[6 0],'w','LineWidth',2)
plot([9 0],[0 0],'w','LineWidth',2)
plot([4.5 4.5],[0 6],'w','LineWidth',2)

plot([0 0.6],[1.9 1.9],'w','LineWidth',2)
plot([0.6 0.6],[1.9 4.1],'w','LineWidth',2)
plot([0.6 0],[4.1 4.1],'w','LineWidth',2)
plot([0 0],[4.1 1.9],'w','LineWidth',2)

aux = 9-0.6;
plot([0+aux 0.6+aux],[1.9 1.9],'w','LineWidth',2)
plot([0.6+aux 0.6+aux],[1.9 4.1],'w','LineWidth',2)
plot([0.6+aux 0+aux],[4.1 4.1],'w','LineWidth',2)
plot([0+aux 0+aux],[4.1 1.9],'w','LineWidth',2)
hold on

for i = 0:9
    plot([i i],[0 6],'k--','LineWidth',0.25)
end

for i = 0:6
    plot([0 9],[i i],'k--','LineWidth',0.25)
end
% Círculos
t = -0.2:0.01:2*pi;
plot(0.75*cos(t)+4.5,0.75*sin(t)+3,'w','LineWidth',2)
%plot(0.05*sin(t)+3,0.05*cos(t)+4.5,'k')
plot(0.1*cos(t)+1.3,0.1*sin(t)+3,'w','LineWidth',2)
plot(0.1*cos(t)+9-1.3,0.1*sin(t)+3,'w','LineWidth',2)
set(gca,'Color',[0 0.6 0.2], 'Xtick',[],'YTick',[]);

axis([-1 10 -1 7])

annotation('textbox',...
    [0.345 0.525 0.0821428571428571 0.0738095238095247],...
    'String',{'$\mathcal{V}_i$'},...
    'LineStyle','none',...
    'Interpreter','latex',...
    'FontSize',18,...
    'FitBoxToText','off',...
    'EdgeColor',[1 1 1]);

annotation('textbox',...
    [0.5 0.85 0.0821428571428571 0.0738095238095247],...
    'String',{'$\Omega$'},...
    'LineStyle','none',...
    'Interpreter','latex',...
    'FontSize',24,...
    'FitBoxToText','off',...
    'EdgeColor',[1 1 1]);