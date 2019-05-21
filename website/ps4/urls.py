from django.urls import path

from . import views

app_name = 'ps4'
urlpatterns = [
    path(r'', views.index, name='home'),
    path(r'home/', views.index, name='home'),
    path(r'download/', views.download, name='download'),
    path(r'report/', views.report, name='report'),
    path(r'login/', views.login, name='login'),
    path(r'register/', views.register, name='register'),
    path(r'account/', views.account, name='account'),
    path(r'backup/', views.backup, name='backup'),
    path(r'backup/add/', views.backup_add, name='backup_add'),
    path(r'backup/view/<int:id>/', views.backup_view, name='backup_view'),
    path(r'backup/view/content/<int:id>/', views.backup_view_content, name='backup_view_content'),
    path(r'backup/download/<int:id>/', views.backup_download, name='backup_download'),
    path(r'logout/', views.logout, name='logout'),
    path(r'about/', views.about, name='about'),
    path(r'terms/', views.terms, name='terms'),
]
