from django.urls import resolve

def nav(request):
    return {'nav': {resolve(request.path_info).url_name: 'active'}}

def title(request):
    return {'title': resolve(request.path_info).url_name.title()}
