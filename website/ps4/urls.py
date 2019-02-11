from django.urls import path

from . import views

app_name = 'ps4'
urlpatterns = [
    path('', views.index, name='index'),
    path('download', views.download, name='download'),
    path('report', views.report, name='report'),
    path('login', views.login, name='login'),
    path('register', views.register, name='register'),
    path('account', views.account, name='account'),
    path('logout', views.logout, name='logout'),
    path('about', views.about, name='about'),
    path('terms', views.terms, name='terms'),
]
