package ru.hatchery.glade.ui;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;
import java.util.NoSuchElementException;

import ru.hatchery.glade.Callable;
import ru.hatchery.glade.GladeObject;
import ru.hatchery.glade.model.UniqueArrayList;
import ru.hatchery.glade.model.Vector3f;
import ru.hatchery.glade.render.Drawable;
import ru.hatchery.glade.ui.layout.Layout;

// TODO Бесконечная рекурсия при попытке смены фокуса, когда focusable элементов вообще нет
public abstract class Widget extends GladeObject
{	
	private List<Widget> children;
	private Widget parent;
	private Layout layout;
	private ListIterator<Widget> focusIterator;
	private Widget breadCrumb;
	private List<Drawable> unfocusedViews = new UniqueArrayList<Drawable>();
	private List<Drawable> focusedViews = new UniqueArrayList<Drawable>();
	private boolean focused = false;
	private boolean focusable = true;
	private List<Callable> onFocusListeners = new ArrayList<Callable>();
	private List<Callable> onBlurListeners = new ArrayList<Callable>();
	private Callable action;
	
	public Widget(Layout layout)
	{
		this.layout = layout;
		children = new UniqueArrayList<Widget>();
	}
	
	@Override
	public void setEnabled(boolean enable)
	{
		super.setEnabled(enable);
		
		for (Widget child: children) {
			child.setEnabled(enable);
		}
	}
	
	public void addFocusedView(Drawable view)
	{
		addDrawable(view);
		toggleView(view, focused);
		focusedViews.add(view);
	}
	
	public void addUnfocusedView(Drawable view)
	{
		addDrawable(view);
		toggleView(view, !focused);
		unfocusedViews.add(view);
	}
	
	public void addFocusedView(List<Drawable> view)
	{
		for (Drawable drawable: view) {
			addFocusedView(drawable);
		}
	}
	
	public void addUnfocusedView(List<Drawable> view)
	{
		for (Drawable drawable: view) {
			addUnfocusedView(drawable);
		}
	}
	
	public boolean isPointInside(float x, float y)
	{
		Vector3f position = getTransform().getPosition();
		Vector3f scale = getTransform().getScale();
		
		if (x > position.x - scale.x && x < position.x + scale.x &&
			y > position.y - scale.y && y < position.y + scale.y
		) {
			return true;
		}
		
		return false;
	}
	
	public Widget getWidgetAtPoint(float x, float y)
	{
		if (isPointInside(x, y)) {
			Iterator<Widget> i = children.iterator();
			Widget child, result;
			
			while (i.hasNext()) { // FIXME Если виджеты наслаиваются друг на друга, то порядок обхода может быть неверным
				child = i.next();
				result = child.getWidgetAtPoint(x, y);
				
				if (result != null && result.isFocusable()) {
					return result;
				}
			}
			
			if (this.isFocusable()) {
				return this;
			}
		}
		
		return null;
	}
	
	public void setFocused(boolean focused)
	{
		for (Drawable drawable: focusedViews) {
			toggleView(drawable, focused);
		}
		
		for (Drawable drawable: unfocusedViews) {
			toggleView(drawable, !focused);
		}

		if (focused && !this.focused) {
			for (Callable listener: onFocusListeners) {
				listener.call();
			}
		} else if (!focused && this.focused) {
			for (Callable listener: onBlurListeners) {
				listener.call();
			}
		}
		
		this.focused = focused;
	}
	
	public Layout getLayout()
	{
		return layout;
	}
	
	public void setParent(Widget widget)
	{
		if (parent != null) {
			parent.getChildren().remove(this);
		}
		
		if (widget != null) {
			widget.getChildren().add(this);
		}
		
		parent = widget;
	}
	
	public Widget getParent()
	{
		return parent;
		
	}
	
	public Widget getFocusedElement()
	{
		if (breadCrumb == null) {
			return null;
		}
		
		if (breadCrumb.equals(this)) {
			return this;
		}
		
		return breadCrumb.getFocusedElement();
	}
	
	/**
	 * Если пройдены все потомки - breadCrumb ставится в null, и возвращается false
	 */
	public boolean focusNextDescendant()
	{
		// снимаем флаг фокуса с текущего сфокусированного элемента (ищем его от корня)
		if (getParent() == null) {
			Widget focusedElement = getFocusedElement();
			
			if (focusedElement != null) {
				getFocusedElement().setFocused(false);
			}
		}
		
		boolean result = true;
		
		// Если сфокусированный элемент - этот, то на этом шаге фокус просто убирается и родитель оповещается что в этой ветке элементы закончились
		if (breadCrumb == this) {
			breadCrumb = null;
			
			// если это корневой элемент - делаем еще одну итерацию (чтобы не случилось так что ни один элемент дерева не имеет фокус)
			if (getParent() == null) {
				focusNextDescendant();
			}
			
			result = false;
		} else {
			// Если фокус был не в нашей ветке, значит нужно создать новый итератор для обхода нашей ветки
			if (breadCrumb == null || focusIterator == null) {
				focusIterator = children.listIterator();
			}
			
			// пробуем переключить фокус у первого прямого потомка. Если потомок возвращает false (т.е. у него закончились его потомки), то пробуем следующего
			while (breadCrumb == null || !breadCrumb.focusNextDescendant()) {
				try {
					Widget newBreadCrumb = focusIterator.next();
					
					if (breadCrumb != null && breadCrumb.equals(newBreadCrumb)) {
						newBreadCrumb = focusIterator.next();
					}
					
					breadCrumb = newBreadCrumb;
				} catch (NoSuchElementException e) { // если наши прямые потомки закончились
					if (!isFocusable()) { // если этот элемент не фокусируемый, то в этой ветке больше нечего фокусировать, оповещаем об этом вызвавшего родителя
						breadCrumb = null;
						
						if (getParent() == null) {
							focusNextDescendant();
						}
						
						result = false;
					} else { // если этот элемент фокусируемый - фокусируем его
						breadCrumb = this;
						result = true;
					}
					
					break;
				}
			}
		}
		
		// Устанавливаем флаг фокуса на новый найденный элемент
		if (getParent() == null) {
			Widget focusedElement = getFocusedElement();
			
			if (focusedElement != null) {
				getFocusedElement().setFocused(true);
			}
		}
		
		return result;
	}
	
	/**
	 * Если пройдены все потомки - breadCrumb ставится в null, и возвращается false
	 */
	public boolean focusPreviousDescendant()
	{
		if (getParent() == null) {
			Widget focusedElement = getFocusedElement();
			
			if (focusedElement != null) {
				getFocusedElement().setFocused(false);
			}
		}
		
		boolean result = true;
		
		if (breadCrumb == this) {
			breadCrumb = null;
			
			if (getParent() == null) {
				focusPreviousDescendant();
			}
			
			result = false;
		} else {
			if (breadCrumb == null || focusIterator == null) {
				focusIterator = children.listIterator(children.size());
			}
			
			while (breadCrumb == null || !breadCrumb.focusPreviousDescendant()) {
				try {
					Widget newBreadCrumb = focusIterator.previous();
					
					if (breadCrumb != null && breadCrumb.equals(newBreadCrumb)) {
						newBreadCrumb = focusIterator.previous();
					}
					
					breadCrumb = newBreadCrumb;
				} catch (NoSuchElementException e) {
					if (!isFocusable()) {
						breadCrumb = null;
						
						if (getParent() == null) {
							focusPreviousDescendant();
						}
						
						result = false;
					} else {
						breadCrumb = this;
						result = true;
					}
					
					break;
				}
			}
		}
		
		if (getParent() == null) {
			Widget focusedElement = getFocusedElement();
			
			if (focusedElement != null) {
				getFocusedElement().setFocused(true);
			}
		}
		
		return result;
	}
	
	public Iterator<Widget> getChildrenIterator()
	{
		return children.iterator();
	}
	
	public boolean hasChildren()
	{
		return !getChildren().isEmpty();
	}
	
	public boolean isFocusable()
	{
		return focusable;
	}
	
	public void setFocusable(boolean focusable)
	{
		this.focusable = focusable;
	}
	
	public List<Widget> getChildren()
	{
		return children;
	}
	
	public void addOnFocusListener(Callable listener)
	{
		onFocusListeners.add(listener);
	}
	
	public void addOnBlurListener(Callable listener)
	{
		onBlurListeners.add(listener);
	}
	
	public void removeOnFocusListener(Callable listener)
	{
		onFocusListeners.remove(listener);
	}
	
	public void removeOnBlurListener(Callable listener)
	{
		onBlurListeners.remove(listener);
	}
	
	public void setAction(Callable action)
	{
		this.action = action; 
	}
	
	public void performAction()
	{
		if (action != null) {
			action.call();
		}
	}
}
