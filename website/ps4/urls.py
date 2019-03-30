from django.urls import path

from . import views

app_name = 'ps4'
urlpatterns = [
    path(r'', views.index, name='index'),
    path(r'home/', views.index, name='home'),
    path(r'download/', views.download, name='download'),
    path(r'report/', views.report, name='report'),
    path(r'login/', views.login, name='login'),
    path(r'register/', views.register, name='register'),
    path(r'account/', views.account, name='account'),
    path(r'logout/', views.logout, name='logout'),
    path(r'about/', views.about, name='about'),
    path(r'terms/', views.terms, name='terms'),
]
